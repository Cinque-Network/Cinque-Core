[![Build Status](https://img.shields.io/github/workflow/status/DigiByte-Core/digibyte/Build%20and%20check/develop)](https://github.com/DigiByte-Core/digibyte/actions/workflows/ci-coverage.yml)
[![Version](https://img.shields.io/github/v/release/DigiByte-Core/digibyte?include_prereleases)](https://github.com/DigiByte-Core/digibyte/releases)
[![Release date](https://img.shields.io/github/release-date/DigiByte-core/digibyte)](https://github.com/DigiByte-Core/digibyte/releases)
[![Status](https://img.shields.io/badge/status-stable-brightgreen)](https://github.com/DigiByte-Core/digibyte/tree/develop)
[![Platforms](https://img.shields.io/badge/platforms-Windows%20%7C%20Linux%20x86_64%20%7C%20ARM%20Linux%20%7C%20OSX-lightgrey)](https://github.com/DigiByte-Core/digibyte/releases)
[![Downloads](https://img.shields.io/github/downloads/DigiByte-Core/digibyte/total?style=social)](https://github.com/DigiByte-Core/digibyte/releases)
[![Repo size](https://img.shields.io/github/repo-size/DigiByte-Core/digibyte)](https://github.com/DigiByte-Core/digibyte)
[![Issues](https://img.shields.io/github/issues-raw/DigiByte-Core/digibyte)](https://github.com/DigiByte-Core/digibyte/issues)
[![Open PRs](https://img.shields.io/github/issues-pr/DigiByte-Core/digibyte)](https://github.com/DigiByte-Core/digibyte/pulls)
[![Closed PRs](https://img.shields.io/github/issues-pr-closed/DigiByte-Core/digibyte)](https://github.com/DigiByte-Core/digibyte/pulls)
[![Commits](https://img.shields.io/github/commit-activity/y/DigiByte-Core/digibyte)](https://github.com/DigiByte-Core/digibyte/commits/develop)
[![Contributors](https://img.shields.io/github/contributors/DigiByte-Core/digibyte)](https://github.com/DigiByte-Core/digibyte/graphs/contributors)
[![Last Commit](https://img.shields.io/github/last-commit/DigiByte-Core/digibyte)](https://github.com/DigiByte-Core/digibyte/graphs/commit-activity)
[![License](https://img.shields.io/github/license/DigiByte-Core/digibyte)](https://github.com/DigiByte-Core/digibyte/blob/develop/COPYING)
[![Gitter](https://badges.gitter.im/DigiByte-Core/protocol.svg)](https://gitter.im/DigiByte-Core/protocol?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

<img width="450" type="image/svg+xml" src="./doc/logo_horizontal_github.svg" />

## What is Cinque?

Cinque (CNQ) is a highly secure, decentralized, distributed and time-tested global blockchain that was founded in early 2022 with a focus on cyber security, payments & secure communications technologies.

For more information, as well as an immediately useable, binary version of the Cinque Core software, see <https://cinque.network>

## Cinque FAQ

**Launch Date**: SOMEDATE, 2022

**Blockchain Type**: Public, Decentralized, UTXO based, Multi-Algorithm

**Ticker Symbol**: CNQ

**Genesis Block Hash**: "To be determined"

**Max Total Supply**: 5 Million Cinque

**Current Supply**: 0 CNQ (Not Launched)

**Current Block Reward**: Less than triple digits CNQ (TBD)

**Mining Algorithms**: Five individual: Equihash 192,7, MYR-GRS, Ghostrider, Keccak & Heavyhash

**Block Interval**: 15 Second Blocks (75 seconds per algo)

**Algo Block Share**: 20% Block Share Per Algo (5)

**Difficulty Retarget**: Every 1 Block, 5 Separate Difficulties, independent difficulty for each Mining Algo

**SegWit Support**: Yes.

**Hardforks**: None Yet.

**Softforks**: TBD.

You can mine Cinque on one of five separate mining algorithms. Each algo averages out to mine 20% of new blocks. This allows for much greater decentralization than other blockchains. An attacker with 99% of of any individual algorithm would still be unable to hardfork the blockchain, making Cinque much more secure against PoW attacks than other blockchains.

## Cinque vs Bitcoin

**Security**:

- 5 Cinque mining algorithms vs. 1 Bitcoin mining algorithm.
- Cinque mining is much more decentralized.
- Cinque mining algorithms can be changed out in the future to prevent centralization.

**Speed**:

- Cinque transactions occur much faster than Bitcoin transactions.
- 1-2 second transaction notifications.
- 15 second Cinque blocks vs. 10 minute Bitcoin blocks.
- Cinque has 6x block confirmations 1.5 minutes vs. 1 hour with Bitcoin.

**Transaction Volume**:

- Cinque can handle many more transactions per second.
- Bitcoin can only handle 3-4 transactions per second.
- Cinque currently can handle 560+ transactions per second.

**Marketability & Usability**:

- Cinque is an easy brand to market to consumers.
- Cinque is also much cheaper to acquire.

## License

Cinque Core is released under the terms of the MIT license. See [COPYING](COPYING) for more information or see <https://opensource.org/licenses/MIT>.

## Development Process

The `develop` branch is regularly built and tested, but is not guaranteed to be completely stable.  Additionally, the develop branch represents ongoing development from which candidate releases will be cut.  The `master` branch represents the current stable version currently in production. [Tags](https://github.com/Cinque-Network/Cinque-COre/tags) are created regularly to indicate new official, stable release versions of Cinque Core.  Finally, `feature/{a-new-feature}` branches represent current development destined for future releases.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

## Testing

### Automated Testing

Developers are required to write [unit tests](src/test/README.md) for new code, and to submit new unit tests for any old code that is changed. Unit tests can be compiled and run (assuming they weren't disabled in configure) with: `make check`. Further details on running and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written in Python, that are run automatically on the build server.  These tests can be run (if the [test dependencies](/test) are installed) with: `test/functional/test_runner.py`

The Travis CI system makes sure that every pull request is built for Windows, Linux, and macOS, and that unit/sanity tests are run automatically.

### Manual Quality Assurance (QA) Testing

Changes are required be tested by somebody other than the developer who wrote the code. This is especially important for large or high-risk changes. A test plan must be included with the pull request description if testing the changes is not straightforward.

Thank you for using Cinque!
