/***************************************************************************************************
 USINGS
***************************************************************************************************/

use std::fmt;
use rand::Rng;
use std::os::raw::c_int;
use std::os::raw::c_char;
use std::fs::File;
use std::io::{prelude::*, BufReader};

/***************************************************************************************************
 CONSTANTS
***************************************************************************************************/

pub const MAX_TEXT_LENGTH: i32 = 50;
pub const ANIMATION_ERROR: &str = "An unexpected animation error occurred.";
const ENDPOINT_SCHEME: &str = "http";
const ENDPOINT_DOMAIN: &str = concat!("ar", "tii", ".hero", "ku", "app.", "com"); // <(.)>  <(.)>  
const ENDPOINT_PORT: i32 = 80;
const ENDPOINT_PATH: &str = "/make?text=";

/***************************************************************************************************
 ENUMS
***************************************************************************************************/

pub enum AnimationType {
    RightToLeft,
    UpperRightToLowerLeft,
    Shake
}

impl Default for AnimationType {
    fn default() -> Self { AnimationType::RightToLeft }
}

/***************************************************************************************************
 STRUCTS
***************************************************************************************************/

#[derive(Default)]
pub struct AsciiArt {
    pub anim_type: AnimationType,
    pub data: Vec<String>,
    pub length: i32,
    pub height: i32,
    _y_coord: c_int
}

/***************************************************************************************************
 ERRORS
***************************************************************************************************/

#[derive(Debug, Clone)]
pub struct HttpRequestError {
    pub code: i32,
    pub message: String
}

impl fmt::Display for HttpRequestError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "HTTP request error.  Code: {}.  Response: {}", self.code, self.message)
    }
}

#[derive(Debug, Clone)]
pub struct FileReadError {
    pub file_name: String,
    pub message: String
}

impl fmt::Display for FileReadError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Could not open file '{}'.  Reason: {}", self.file_name, self.message)
    }
}

#[derive(Debug, Clone)]
pub struct AnimationError {
    pub message: String
}

impl fmt::Display for AnimationError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Could not animate art.  Reason: {}", self.message)
    }
}

/***************************************************************************************************
 EXTERN FUNCTIONS
***************************************************************************************************/

extern "C" {
    fn sl_initialize();
    fn sl_draw(y: c_int, x: c_int, character: c_char) -> c_int;
    fn sl_wait();
    fn sl_finalize();
    fn sl_get_cols() -> c_int;
    fn sl_get_lines() -> c_int;
}

/***************************************************************************************************
 FUNCTIONS
***************************************************************************************************/

impl AsciiArt {

    pub fn get_from_web(text: &str) -> Result<AsciiArt, reqwest::Error>
    {
        let mut uri = ENDPOINT_SCHEME.to_owned();
        uri.push_str("://");
        uri.push_str(ENDPOINT_DOMAIN);
        uri.push_str(":");
        uri.push_str(ENDPOINT_PORT.to_string().as_str());
        uri.push_str(ENDPOINT_PATH);
        uri.push_str(text);

        let resp = reqwest::blocking::get(uri)?;
        let body = resp.text()?;
        let lines = body.split("\n");
        let mut result: AsciiArt = Default::default();
        let mut max_len: usize = 1;

        for line in lines {
            let mut l = line.to_string();
            l.push_str(" ");
            if l.len() > max_len { max_len = l.len(); }
            result.data.push(l);
        }
        
        result.height = result.data.len() as i32;
        result.length = max_len as i32;
        Ok(result)
    }

    pub fn get_from_file(file_name: &str) -> Result<AsciiArt, FileReadError>
    {
        let file = match File::open(file_name) {
            Ok(f) => f,
            Err(e) => { return Err(FileReadError { file_name: file_name.to_string(), message: e.to_string() }); }
        };

        let reader = BufReader::new(file);
        let mut result: AsciiArt = Default::default();
        let mut max_len: usize = 1;

        for line in reader.lines() {
            match line {
                Ok(mut l) => {
                    l.push_str(" ");
                    if l.len() > max_len { max_len = l.len(); }
                    result.data.push(l);
                },
                Err(e) => { return Err(FileReadError { file_name: file_name.to_string(), message: e.to_string() }); }
            }
        }

        result.height = result.data.len() as i32;
        result.length = max_len as i32;
        Ok(result)
    }

    pub fn animate(&mut self) -> Result<bool, AnimationError> {
        if self.data.len() == 0 || self.length == 0 || self.height == 0 {
            return Ok(false);
        }
        
        unsafe { sl_initialize(); }
        
        let cols: c_int;
        unsafe { cols = sl_get_cols(); }
        if cols < 1 { return Err(AnimationError { message: ANIMATION_ERROR.to_string()}); }
        let lines: c_int;
        unsafe { lines = sl_get_lines(); }
        if lines < 1 { return Err(AnimationError { message: ANIMATION_ERROR.to_string()}); }
        
        let mut x = cols - 1;
        while x > -self.length {
            if let Err(e) = self.draw(x, cols, lines) {
                unsafe { sl_finalize(); }
                return Err(e);
            }
            unsafe { sl_wait(); }
            x -= 1;
        }
        
        unsafe { sl_finalize(); }
        Ok(true)
    }

    fn draw(&mut self, mut x: c_int, cols: c_int, lines: c_int) -> Result<bool, AnimationError> {
        let mut y: c_int;
        let mut x_start = x;

        match self.anim_type {
            AnimationType::RightToLeft => {
                y = (lines / 2) - (self.height / 2); // the image will be vertically centered
            },
            AnimationType::UpperRightToLowerLeft => {
                if x % 4 == 0 {
                    self._y_coord += 1; // will move image downward
                }

                y = self._y_coord;
            },
            AnimationType::Shake => {
                if x % 3 != 0 { return Ok(false); }; // don't shake too much!

                x = (cols / 2) - (self.length / 2);
                y = (lines / 2) - (self.height / 2);
                let mut rng = rand::thread_rng();
                let r = rng.gen_range(0..5);

                match r {
                    0 => { },        // do nothing, leave the image centered
                    1 => {           // move image up and left
                        x -= 1;
                        y -= 1;
                    },
                    2 => {           // move image up and right
                        x += 1;
                        y -= 1;
                    },
                    3 => {           // move image down and left
                        x -= 1;
                        y += 1;
                    },
                    4 => {           // move image down and right
                        x += 1;
                        y += 1;
                    },
                    _ => {}
                }

                x_start = x;
            }
        }

        let mut char_idx = 0;
        let mut i = 0;

        while i < self.height {
            let line = self.data[i as usize].as_str();

            while char_idx < line.len() {
                let c = line.chars().nth(char_idx).unwrap() as c_char; 
                unsafe { sl_draw(y+i, x, c); }
                char_idx += 1;
                x += 1;
            }
            
            x = x_start;
            char_idx = 0;
            i += 1;
        }

        return Ok(true);
    }
}
