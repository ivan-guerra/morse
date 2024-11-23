pub struct Config {
    pub input_ascii: Option<std::path::PathBuf>,
    pub output_ascii: Option<std::path::PathBuf>,
    pub disable_audio: bool,
    pub print_code: bool,
    pub pitch: u32,
    pub duration: u32,
}

impl Config {
    pub fn new(
        input_ascii: Option<std::path::PathBuf>,
        output_ascii: Option<std::path::PathBuf>,
        disable_audio: bool,
        print_code: bool,
        pitch: u32,
        duration: u32,
    ) -> Self {
        Self {
            input_ascii,
            output_ascii,
            disable_audio,
            print_code,
            pitch,
            duration,
        }
    }
}
