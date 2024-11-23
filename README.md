# morse

https://github.com/ivan-guerra/morse/assets/13009513/c7319085-0e97-43f5-aa31-e31adcb70790

This project implements a text to Morse code translator. The tool is able to
take ASCII text and transform it to Morse code audio or a dots/dashes text
representation.

### Program Usage

Program usage is as follows:

```text
Convert ASCII text to Morse code audio and text.

Usage: morse [OPTIONS]

Options:
  -i, --input-ascii <INPUT_ASCII>    path to an input ASCII text file
  -o, --output-ascii <OUTPUT_ASCII>  path to output Morse coded input
  -d, --disable-audio                disable Morse code audio player
  -p, --print-code                   print Morse code to console
  -u, --pitch <PITCH>                a integer percentage value, the higher the percentage the higher the pitch of each dot/dash [default: 50]
  -l, --duration <DURATION>          a integer percentage value, the higher the percentage the longer of each dot/dash tone lasts [default: 80]
  -h, --help                         Print help
  -V, --version                      Print version
```

When using the `--print-code` or `--output-ascii` option, the utility will
output a Morse code translation in the form of dots and dashes. Letters are
separated by spaces and words by `/`. If a letter cannot be translated a `#`
will appear in the output.
