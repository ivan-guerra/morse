# morse

https://github.com/ivan-guerra/morse/assets/13009513/c7319085-0e97-43f5-aa31-e31adcb70790

This project implements a text to Morse code translator. The tool is able to
take ASCII text and transform it to Morse code audio or a dots/dashes text
representation.

### Building

To build the project locally, you will need the following libraries and tools
installed:

* CMake3.13+
* C++ compiler supporting C++20 features
* SDL2 Developer Libraries

To build the project, change directory to the `scripts/` directory and run
`build.sh`

```bash
./build.sh
```
> **Note**
> `build.sh` supports additional option flags for building the project docs
> (requires [Doxygen][1]) and unit tests. Run `build.sh -h` to see all the
> options.

After a successful build, you will find the binary installed to
`morse/bin/morse`.

### Program Usage

Program usage is as follows:

```text
usage: morse [OPTION]...
convert ascii text to Morse code text and audio

OPTIONS
	-i,--input-ascii FILE
		path to an input ASCII text file
	-o,--output-ascii FILE
		path to output Morse coded input
	-d,--disable-audio
		disable Morse code audio player
	-p,--print-code
		print Morse encoding to STDOUT
	-u,--pitch NUM
		a integer percentage value in the range [0, 100], the higher the
		percentage the higher the pitch of each dot/dash (default 50)
	-l,--duration NUM
		a integer percentage value in the range [0, 100], the higher the
		percentage the longer each dot/dash tone lasts (default 25)
	-h,--help
		print this help message
```

When using the `--print-code` or `--output-ascii` option, the utility will
output a Morse code translation in the form of dots and dashes. Letters are
separated by spaces and words by `/`. If a letter cannot be translated a `#`
will appear in the output.

### Running the Tests

`morse` has been unit tested using the GoogleTest framework in conjunction with
`ctest`. Be sure to run submodule init/update or clone the repo using the
`--recursive` flag to pull down GoogleTest sources.

To build the unit tests, run the build script as before with the addition of the
`-t` flag:

```bash
./build.sh -t
```

To run the test, change directory to `morse/build` and run `ctest`.

[1]: https://www.doxygen.nl/
