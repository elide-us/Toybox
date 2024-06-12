local json = require('dkjson')

local layer = createLayer()
local fontSize = 24
local font = loadFont("Play",fontSize)
local rX,rY = getResolution()
local col1 = 10
local col2 = (rX/2)+10


local message = json.decode(getInput()) or {}

local industry=message["I"]
local targets=message["T"]
local output=message["O"]

offsetY = fontSize/2+20
addText(layer,font,"Metalwork: "..industry["schematic"],col2,offsetY)
addText(layer,font,"State: "..industry["state"],col1,offsetY)

offsetY = fontSize*2+20
addText(layer,font,"Targets:",col1,offsetY)
offsetY = offsetY+fontSize+2
for i=1,#targets do
    addText(layer,font,targets[i]["id"],col1,offsetY)
    addText(layer,font,targets[i]["qty"],col1+400,offsetY)
    offsetY = offsetY+fontSize+2
end

offsetY = fontSize*2+20
addText(layer,font,"Output:",col2,offsetY)
offsetY = offsetY+fontSize+2
for i=1,#output do
    addText(layer,font,output[i]["id"],col2,offsetY)
    addText(layer,font,output[i]["qty"],col2+400,offsetY)
    offsetY = offsetY+fontSize+2
end

requestAnimationFrame(1)