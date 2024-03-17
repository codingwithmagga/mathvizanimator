# Pull Request Template

**IMPORTANT: Please do not create a Pull Request without creating an issue first.**

*Any change needs to be discussed before proceeding. Failure to do so may result
in the rejection of the pull request.*

Please provide enough information so that others can review your pull request. You
can skip this if you're fixing a typo or adding an app to the Showcase.

Explain the **details** for making this change. What existing problem does the pull
request solve?

Ex:

1. If you "Added a/changed the function to do X", explain why:

    * it is necessary to have a way to do X.
    * if there already exists a way, why is your implementation better

2. If you "Fixed bug/error in X", explain:

    * what was the bug/error (if you already made an issue, please link to it here)
    * how does your implementation fix the issue

## Code style and formatting

Check the [Contributors Style Guidelines section](CONTRIBUTING.md#Style-guidelines)
for how to write your code and the [Contributors Code Formatting section](CONTRIBUTING.md#Code-formatting)
for how to format your code.

### Closing Issues

Put `Fixes #<ISSUE_NUMBER>` or similar in your comment to auto-close the issue that your PR fixes
(if such).

---

## Template

Please delete all the above lines (including this one) and replace <ISSUE_NUMBER> with the issue you are fixing with this PR. Fill out the topics below if necessary.

Fixes #<ISSUE_NUMBER>

### Proposed changes

*
*
*

### Motivation behind changes

A short text about the motivation of the code changes.

### Test plan

Demonstrate the code is solid. Example: The exact commands you ran and their output,
screenshots / videos if the pull request changes UI.

*Make sure tests pass on all of the [relevant CI workflows](https://github.com/codingwithmagga/mathvizanimator/actions).*

### Pull Request Readiness Checklist

See details at [CONTRIBUTING.md](CONTRIBUTING.md).

* [ ] I agree to contribute to the project under MathVizAnimator (GNU General Public License v3.0)
[License](LICENSE).

* [ ] To the best of my knowledge, the proposed patch is not based on a code under
a license that is incompatible with MathVizAnimator.

* [ ] The PR is proposed to proper branch.

* [ ] There is reference to original bug report and related work.

* [ ] There is a unit test and or integration test added if necessary.

* [ ] The feature is well documented and an example was added if necessary.
