# rs_switcher
WIP/graveyard

plan is to send a click event anytime your mouse changes angles quickly enough. Current impl just uses graph quadrants which is really bad. need to calc a moving average w/ just the directional component of a polar vector, and if the last N updates are far enough from the average, we send a click and continue updating the moving average. A queue that stores the last N updates would probably work nicely for this. That or just keep track of two moving averages over N updates and X updates, with x being smaller, and if the moving average of X is sufficiently different than that of N, we send a click
