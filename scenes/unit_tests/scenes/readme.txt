Organization of unit tests:

- All unit tests will be grouped into folders by integrator
- Those folders will then be broken up into subfolders named after what is being tested
- Example:  whitted/diffuse/test_###.json
--  This unit test would test the diffuse BSDF for the whitted integrator
--  The numbering scheme is assigned in order chronological order of when the unit test was implemented

Current TODOs:

finish creating new directory structure
all volpath scenes are weird / not generated. Look into that
whitted sdf is not generated / broken
Remove directory structure, going to make a nice website viewer instead