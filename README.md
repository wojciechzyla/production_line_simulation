# production_line_simulation

A production line in a factory consisting of components combined into a coherent whole:
* unloading ramps - deliver semi-finished products to factories
* workers - process semi-finished products
* storehouses - they store semi-finished products through the entire technological process (creating a network outlet, sinks)

Links are allowed as part of the technological process:
* unloading ramp → worker (i.e. delivery of semi-finished products to the first stage of production)
* worker → worker (i.e. providing a semi-finished product for evaluation)
* worker → warehouse (i.e. placing the final semi-finished product in the warehouse)

Each ramp and each worker can only transfer semi-finished products to a certain subset of all receivers (workers and warehouses) - this subset is defined individually for each ramp / worker.
 
 
 
Additional notes:
* Each semi-finished product delivered to the factories has its own serial number, which does not change between production stages (this number is a positive integer).
* Deliveries of semi-finished products to the ramp take place with a period (eg every second round of simulation), beginning from first round. Only one unit of semi-finished product is delivered at a time.
* A worker processes at once (in a given turn) only one unit of semi-finished product, with each worker taking specified number of turns to process (it can differ between workers, but it is always at least one turn).
* Each worker has his preferences regarding the order of semi-finished products delivered to him - e.g. some prefer to process them in the order of arrival (first in, first out), others prefer to start with the semi-finished product delivered last (last in, first out). At the moment, workers are processing products only in the FIFO or LIFO mode, but in the future they will definitely want to develop new work organization schemes ...
* The worker does not stop working on the semi-finished product as new come into him - these are simply inserted into the queue and await the worker's completion of the current task.
* At the moment, each ramp/worker transfers processed semi-finished product to one of the assigned recipients without favoring any of them (i.e. the recipient is selected from a pool of n receivers with the same probability - 1/n). Ultimately, however, each ramp/worker will have the possibility to have their own preferences (e.g. they will pass semi-finished products to worker A twice as often as worker B) - this requirement should be taken into account at the design stage, using an appropriate data structure to store information about the preferences of recipients' choice.
* Each warehouse has the ability to store products independently of other warehouses (i.e. the internal implementation of the functionality of the "semi-finished products container" may differ between warehouses).

