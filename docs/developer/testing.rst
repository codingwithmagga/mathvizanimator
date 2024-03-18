Testing
=======

All non-abstract classes should have associated unit tests. To track the current Code Coverage the website `CodeCov <https://app.codecov.io/gh/codingwithmagga/mathvizanimator>`_ is used. The code coverage should be as high as possible.
To fulfil this goal also integration tests are added to this project, which maybe could also be seen as End-to-end tests or gui tests. Using the test class **TestHelperFunctions**, different user interactions in the gui can be simulated. In general it should be the goal, that every possible user interaction is backed up by a so called integration test.