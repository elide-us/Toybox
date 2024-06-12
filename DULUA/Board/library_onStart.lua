json=require("dkjson")
printf=string.format

c_retryTimer = 45*db.getIntValue("multi")
c_tickRate = 5
c_debug = false

function Debug(msg)
    if c_debug then
        system.print(msg)
    end
end
function GetItemName(id)
    local name = system.getItem(id)["displayName"]
    return name
end
function GetSchematicName(ptr)
    local id = ptr.getOutputs()[1]["id"]
    local name = GetItemName(id)
    return name
end

Item = {}
Item.__index = Item
function Item:new(id,qty)
    local self = {}
    self.id = id
    self.name = GetItemName(id)
    self.qty = qty
    function self:serialize()
        local serial = {}
        serial["id"]=self.id
        --serial["name"]=self.name
        serial["qty"]=self.qty
        return serial
    end
    return setmetatable(self,Item)
end

function GetDelay()
    if db.hasKey("delay") then
        local v = db.getIntValue("delay")
        db.setIntValue("delay",v+1)
    else
        db.setIntValue("delay",1)
    end
    local d = db.getIntValue("delay")
    Debug("Delay: "..d)
    return d
end

Container = {}
Container.__index = Container
function Container:new(ptr)
    local self = {}
    self.ptr = ptr
    self.volume = self.ptr.getItemsVolume()
    self.maxVolume = ptr.getMaxVolume()
    self.percent = (self.volume/self.maxVolume)*100
    self.lastVolume = 0
    self.requestUpdate = true
    self.retryTimer = 45 * GetDelay()
    self.contents = {}
    function self:updateContent()
        local ttl = self.ptr.updateContent()
        --if ttl > 0 then
            --Debug("Next update in "..ttl.." seconds.")
        --end
    end
    function self:onContentUpdate()
        Debug("Container:onContentUpdate()")
        self.contents = {}
        local content = self.ptr.getContent()
        if content ~= nil then
            for i=1,#content do
                self.contents[#self.contents+1]=Item:new(content[i]["id"],content[i]["quantity"])
            end
            self.requestUpdate = false
        end
    end
    function self:delete()
        if db.hasKey("delay") then
            local v = db.getIntValue("delay")
            if v > 1 then
                db.setIntValue("delay",v-1)
            else
                db.clearValue("delay")
            end
        end
        local d = db.getIntValue("delay")
        Debug("Delay: "..d)
    end
    function self:update()
        Debug("Container:update()")
        if self.retryTimer <= 0 then
            self.retryTimer = c_retryTimer
            self.volume = self.ptr.getItemsVolume()
            if self.lastVolume ~= self.volume then
                self.requestUpdate = true
                self.lastVolume = self.volume
            end
            if self.requestUpdate then
                self:updateContent()
            end
        else
            self.retryTimer = self.retryTimer-c_tickRate
        end
    end
    function self:serialize()
        local serial = {}
        for i=1,#self.contents do
            serial[i]=self.contents[i]:serialize()
        end
        return serial
    end
    return setmetatable(self,Container)
end

States = {}
States[1]="Stopped"
States[2]="Running"
States[3]="Stalled"
States[4]="Jammed"
States[5]="Outbox"
States[6]="Complete"
States[7]="Schematics"

Internal = {}
Internal[1]="Running"
Internal[2]="Error"
Internal[3]="Waiting"

Industry = {}
Industry.__index = Industry
function Industry:new(ptr)
    local self = {}
    self.ptr = ptr
    self.internal = 3
    self.state = States[self.ptr.getState()]
    self.schematic = GetSchematicName(self.ptr)
    function self:setOutput(id,qty)
        Debug("Industry:setOutput("..GetItemName(id)..","..qty..")")
        self.ptr.setOutput(id)
        self.ptr.startMaintain(qty)
        self.schematic = GetSchematicName(self.ptr)
        self.internal = 1
    end
    function self:update()
        Debug("Industry:update()")
        local s = self.ptr.getState()
        self.state = States[s]
        if s == 7 then
            Debug("Industry State: No schematics")
            self.ptr.stop(false,false)
            self.internal = 2
        elseif s == 6 then
            Debug("Industry State: Monitor for Maintain")
            self.ptr.stop(false,false)
            self.internal = 3
        elseif s == 5 then
            Debug("Industry State: Output not conencted")
            self.ptr.stop(false,false)
            self.internal = 2
        elseif s == 4 then
            Debug("Industry State: Output full")
            self.ptr.stop(false,false)
            self.internal = 2
        elseif s == 3 then
            Debug("Industry State: Insufficient input")
            self.ptr.stop(false,false)
            self.internal = 2
        elseif s == 2 then
            Debug("Industry State: Running")
            self.internal = 1
        elseif s == 1 then
            Debug("Industry State: Idle")
            self.internal = 3
        end
    end
    function self:serialize()
        local serial = {}
        serial["state"]=self.state
        serial["schematic"]=self.schematic
        return serial
    end
    return setmetatable(self,Industry)
end

Targets = {}
Targets.__index = Targets
function Targets:new()
    local self = {}
    self.targets = {}
    function self:addTarget(id,qty)
        Debug("Targets:addTarget("..GetItemName(id)..","..qty..")")
        self.targets[#self.targets+1] = Item:new(id,qty)
    end
    function self:serialize()
        local serial = {}
        for i=1,#self.targets do
            serial[i]=self.targets[i]:serialize()
        end
        return serial
    end
    return setmetatable(self,Targets)
end

FactoryMake = {}
FactoryMake.__index = FactoryMake
function FactoryMake:new(industry,output,db)
    system.print("Initializing FACTORYMAKE ¯\\_(ツ)_/¯")
    local self = {}
    self.industry = Industry:new(industry)
    self.output = Container:new(output)
    --self.input = Container:new(input)
    self.targets = Targets:new()
    self.lastId = 0
    self.lastQty = 0
    self.workFound = false
    self.db = db
    function self:delete()
        self.output:delete()
    end
    function self:addTarget(id,qty)
        self.targets:addTarget(id,qty)
    end
    function self:setOutput(id,qty)
        self.industry:setOutput(id,qty)
    end
    function self:update()
        self.output:update()
        if self.output.requestUpdate == false and #self.targets.targets > 0 then
            if self.industry.internal == 3 then
                Debug("Internal State: Waiting")
                local found = false
                for i=1,#self.targets.targets do
                    if found ~= true then
                        local target = self.targets.targets[i]
                        local match = false
                        for j=1,#self.output.contents do
                            if found ~= true then
                                local item = self.output.contents[j]
                                if item["id"] == target["id"] then
                                    match = true
                                    if item["qty"] < target["qty"] then
                                        Debug("Work found")
                                        self.industry:setOutput(target["id"],target["qty"])
                                        self.industry.internal = 1
                                        found = true
                                    end
                                end
                            end
                        end
                        if match ~= true then
                            Debug("Work found")
                            self.industry:setOutput(target["id"],target["qty"])
                            self.industry.internal = 1
                            found = true
                        end
                    end
                end
                if found == false then
                    Debug("No work found")
                end
            elseif self.industry.internal == 2 then
                Debug("Internal State: Error")
                self.industry:setOutput(self.lastId,self.lastQty)
                self.industry.internal = 1
            elseif self.industry.internal == 1 then
                Debug("Internal State: Running")
            end
            self.industry:update()
        end
        local serial = {}
        serial["I"]=self.industry:serialize() -- State and schematic
        serial["T"]=self.targets:serialize() -- Target Item list
        serial["O"]=self.output:serialize() -- Output Item list
        local input = json.encode(serial)
        screen.setScriptInput(input)
    end
    return setmetatable(self,FactoryMake)
end