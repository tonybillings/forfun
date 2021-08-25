/***************************************************************************************************
 MODULES
***************************************************************************************************/

mod cli;
mod logo;
mod ascii;

/***************************************************************************************************
 USINGS
***************************************************************************************************/

use rand::Rng;
use cli::*;
use logo::*;
use ascii::*;

/***************************************************************************************************
 FUNCTIONS
***************************************************************************************************/

fn get_ascii_art(args: Option<CliArgList>) -> AsciiArt {
    if args.is_none() {
        let mut rng = rand::thread_rng();
        return get_logo_as_ascii_art(rng.gen_range(1..4));
    }

    let mut arg_list: CliArgList = args.unwrap();
    let empty: AsciiArt = Default::default();

    if let Some(t) = arg_list.get("-t")  {
        match AsciiArt::get_from_web(t) {
            Ok(art) => { return art; },
            _ => { return empty; }
        }
    }

    if let Some(f) = arg_list.get("-f")  {
        match AsciiArt::get_from_file(f) {
            Ok(art) => { return art; },
            _ => { return empty; }
        }
    }

    if let Some(n) = arg_list.get("-n") {
        if let Ok(num) = n.parse::<i32>() {
            return get_logo_as_ascii_art(num);
        }
    }

    return empty;
}

fn main() {
    let args = std::env::args().collect::<Vec<String>>();
    let mut art: AsciiArt;

    if args.len() > 1 {
        let arg_list = match CliArgList::from_main(&args) {
            Ok(al) => al,
            Err(e) =>  { println!("{}", e.message); return; }
        }; 
        
        art = get_ascii_art(Some(arg_list));
    } else {
        art = get_ascii_art(None);
    }

    if let Err(m) = art.animate() {
        println!("{}", m.message);
    }
}
