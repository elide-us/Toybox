--ABOUT

FactoryMake is my take on Industry unit automation. You program in a set of items and quantities that 
you want the machine to maintain and it monitors the destination box and tries to keep them full up.

To set this up you'll need:
- Programming Board
- Databank
- Screen (any kind)
- Industry Unit (any kind)
- Output Container

Deploy your container, machine and programming devices. Connect everything to the programming board.
If you're using Container Hub, that's fine, just make sure everything is connected to the proper slot.

The slot setup is as follows:

- slot1: outbox (Container/Container Hub)
- slot2: machine (Industry Unit)
- slot3: db (Databank)
- slot4: screen (Screen)

In the Board folder, you'll find all of the scripts that need to be set up on the programming board.

- library_onStart.lua: Add code to library.onStart() filter
- unit_onStart.lua: Add code to unit.onStart() filter
- unit_onStop.lua: Add code to unit.onStop() filter
- unit_onTimer_Tick.lua: Add code to unit.onTimer() filter with argument "Tick" (without quotes)
- outbox_onContentUpdate.lua: Add code to outbox.onContentUpdate() filter

Make sure the screen is turned on and then activate the programming board. If everything is set up 
correctly the Industry Unit's recipe and status, the container and your targets list should be 
displayed immediately. Every ~30 seconds a job runs to collect the contents of one of your containers. 
If it fails to index the contents, it will try again later. Until the contents aa few minutes to populate 
the container contents, so be patient.

To set up what your production targets will be, you'll edit the unit_onStart script, or unit.onScript() 
filter if you've already imported the code. You'll need to provide the itemId (look it up online) and 
the target quantity for each material you want produced.

For example, you might add the following code for these different machines (only one at a time!):

t:addTarget(2565702107,2000) --Silumin (This item requires a Smelter be connected to the machine slot)
t:addTarget(645870905,2000) --Concrete (This item requires a Refiner be connected to the machine slot)
t:addTarget(2014531313,2000) --Polycarb (This item requires Chem Ind. be connected to the machine slot)

--ROADMAP

- v0.7 - Additional machine details display

- v0.8 - Pretty-up display graphics

- v0.9 - Refactor

- v1.0RC1 - Public

--CHANGELOG

FactoryMake v0.6
- State management logic

FactoryMake v0.5
- Refactor

FactoryMake v0.4
- Targets data and display

FactoryMake v0.3
- Screen display POCs

FactoryMake v0.2
- Device data formatting for screen input

FactoryMake v0.1
- Raw string data from devices
