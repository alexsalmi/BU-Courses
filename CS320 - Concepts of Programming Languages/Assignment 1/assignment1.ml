(*  This function opens the input file, and converts it to a string list, 
    where each element of the list is one line of the file *)
let readfile (path : string) : string list =
    let rec readfileloop (ch : in_channel) : string list = 
        match input_line ch with  
        | s -> s :: readfileloop ch
        | exception End_of_file -> []  
    in let ch = open_in path
    in let lines = readfileloop ch
    in let () = close_in ch
    in lines
;;

(*  This function checks if a string is a pangram, by checking if the string 
    contains each letter of the alphabet, one by one *)
let rec isstringpangram (str: string) : string = 
    let chars = "abcdefghijklmnopqrstuvwxyz" in
    let rec checkletterloop (i: int) : string =
        if i < 0
        then "true" 
            else if String.contains str (String.get chars i)
            then checkletterloop (i-1)
            else "false"
    in checkletterloop (String.length chars - 1)
;;

(*  This function loops through a list of strings, testing if each string is a pangram
    using the isstringpangram function, and then returns a bool list indicating if
    each string was a pangram or not *)
let checklines (strings: string list) : string list = 
    let rec checklinesloop (lines: string list) : string list = 
        match lines with
        | [] -> []
        | head :: rest -> isstringpangram (head) :: checklinesloop (rest)
    in let result = checklinesloop strings
    in result
;;

(*  This function opens the output file, and prints each string in the input
    string list to the file, each as its own line *)
let rec writefile (path : string) (strings : string list ) : unit =
    let rec loop (ch) (strings : string list ) : unit =
        match strings with
        | [] -> ()
        | head :: rest -> let _ = Printf.fprintf ch "%s\n" head in loop ch rest
    in let ch = open_out path 
    in let ()  = loop ch strings 
    in let () = close_out ch in
    ()
;;

(*  Main function. Takes in a tuple of an input file path and an output file path.
    Uses helper functions to determine if each line in the input file is a pangram,
    and prints a bool list to the output file indicating which lines were indeed
    pangrams *)
let pangram (files: string * string) : unit =
    let (infile, outfile) = files in
    let lines = readfile infile in
    let results = checklines lines in
    writefile outfile results
;;