(* --- TYPE DEFINITIONS --- *)
(* com list is used to represent the set of commands *)
type com = 
      PushI of string
    | PushS of string
    | PushN of string
    | PushB of string
    | Push of string
    | Pop 
    | Add
    | Sub
    | Mul
    | Div
    | Rem
    | Neg
    | Swap
    | Concat
    | And
    | Or
    | Not
    | Equal
    | LessThan
    | Bind
    | If
    | Begin
    | End
    | Call
    | Function of (string * string * com list)
    | Quit
;;

(* const list is used to represent the stack *)
type const = 
      Int    of int
    | Bool   of bool
    | String of string
    | Name   of string
    | Unit
    | Funct of (string * com list)
    | Error
;;

(* binding list is used to represent the current environment's bindings *)
type binding = Pair of (string * const)

(* --- HELPER FUNCTIONS FROM LAB/PAST ASSIGNMENTS --- *)
(* reads content of a file into a string list  *)
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

(* writes content of a string list into a file *)
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

(* separates a string into a char list *)
let explode (s:string) : char list =
    let rec expl i l =
        if i < 0 
        then l
        else expl (i - 1) (String.get s i :: l)
    in expl (String.length s - 1) []
;;

(* combines a char list into a string *)
let implode (cl:char list) : string = 
    String.concat "" (List.map (String.make 1) cl)
;;

(* helper funtion for take_while *)
let rec take_while' (p:char -> bool) (es : char list) : (char list) * (char list) = 
    match es with
    | []      -> ([],[])
    | x :: xs -> if p x 
                then let (chars, rest) = take_while' p xs in (x :: chars, rest) 
                else ([],es) (* the lab8_before.ml contained a small bug here *)
;;

(* separates string into a (string*string), delimiter determined by funtion p *)
let take_while (p:char -> bool) (s:string) : string * string = 
    let (echars, erest) = take_while' p (explode s) 
    in (implode echars, implode erest)
;;

(* drops first char of a string *)
let drop_first (s:string) : string = 
    match explode s with
    | [] -> ""
    | _ :: rest -> implode rest
;;

(* converts a string into an int option *)
let parse_int (s : string) : int option = 
    match int_of_string (String.trim s) with
    | n -> Some n
    | exception _ -> None
;;

(* checks if string 'str' cointains the string 'cmdtype' *)
let rec contains (str : string) (cmdtype: string) : bool =
    let rec helper (i: int) : bool =
        if i < 0 
        then true 
        else if (String.get str i) == (String.get cmdtype i)
            then helper (i-1)
            else contains (drop_first str) cmdtype
    in 
    if (String.length str) < (String.length cmdtype)
    then false
    else helper (String.length cmdtype - 1)
;;



(* --- PARSING FUNCTIONS --- *)
(* parses a Push* command, converting into a com type *)
let parse_push (str: string) : com =
    let (cmd,data_raw) = take_while (fun c -> c != ' ') str in
    let data = drop_first data_raw in
    if String.length cmd == 4
    then Push data
    else let push_type = String.get cmd 4 in
        if      push_type == 'I' then PushI data
        else if push_type == 'S' then PushS data
        else if push_type == 'N' then PushN data
        else if push_type == 'B' then PushB data
        else Push "<error>"
;;

(* parses a command, returning correct com type *)
let parse_cmd_str (cmd : string) : com =
    if      contains cmd "Push"     then parse_push cmd
    else if contains cmd "Pop"      then Pop
    else if contains cmd "Add"      then Add
    else if contains cmd "Sub"      then Sub
    else if contains cmd "Mul"      then Mul
    else if contains cmd "Div"      then Div
    else if contains cmd "Rem"      then Rem
    else if contains cmd "Neg"      then Neg
    else if contains cmd "Swap"     then Swap
    else if contains cmd "Concat"   then Concat
    else if contains cmd "And"      then And
    else if contains cmd "Or"       then Or
    else if contains cmd "Not"      then Not
    else if contains cmd "Equal"    then Equal
    else if contains cmd "LessThan" then LessThan
    else if contains cmd "Bind"     then Bind
    else if contains cmd "If"       then If
    else if contains cmd "Begin"    then Begin
    else if contains cmd "End"      then End
    else if contains cmd "Call"     then Call
    else Push "<error>"
;;

let rec parse_fun_cmds (ls : string list) : (com list * string list)=
    match ls with 
    | [] -> ([],[])
    | hd::tl -> if contains hd "Quit" 
                then (Quit :: [], [])
                else if contains hd "FunEnd"
                     then ([], tl)
                     else let (cmds,rest) = parse_fun_cmds tl in
                              (parse_cmd_str hd :: cmds, rest)
;;

let parse_fun_def (str:string) (cmds: string list) : (com * string list) = 
    let (cmd,params_raw) = take_while (fun c -> c != ' ') str in
    let params = drop_first params_raw in
    let (fname,input_raw) = take_while (fun c -> c != ' ') params in
    let input = drop_first input_raw in
    let (fun_cmds,rem_cmds) = parse_fun_cmds cmds in
    (Function (fname,input,fun_cmds), rem_cmds)
;;

(* convert a list of command string into a com list *)
let rec parse_cmds (ls : string list) : com list = 
    match ls with 
    | [] -> []
    | hd::tl -> if contains hd "Quit" 
                then Quit :: []
                else if contains hd "Fun"
                     then (let (fun_com,rem_com) = parse_fun_def hd tl in
                            fun_com :: parse_cmds rem_com)
                     else parse_cmd_str hd :: parse_cmds tl
;;



(* --- EVALUATION FUNCTIONS --- *)
(* converts a string into an Int const *)
let convert_int (str : string) : const =
    match parse_int str with
    | None -> Error
    | Some i -> Int i
;;

(* converts a string into a String Const *)
let convert_string (str : string) : const =
    let chars = List.filter (fun p -> p != '"') (explode str)
    in String (implode chars)
;;

(* converts a string into a Bool const *)
let convert_bool (str : string) : const =
    if      contains str "<true>"  then Bool true
    else if contains str "<false>" then Bool false
    else Error
;;

(* converts a string into a Unit const *)
let convert_unit (str : string) : const =
    if contains str "<unit>" 
    then Unit
    else Error
;;

(* pop the top element of the stack *)
let eval_pop (stack : const list) : const list =
    match stack with
    | [] -> Error :: stack
    | hd::tl -> tl
;;

(* gets the value bound to Name 'name', returning Error if 'name' is unbound *)
let rec get_binding_value (env : binding list) (name : string) : const = 
    match env with
    | [] -> Error
    | Pair (name', value) :: tl -> if name' = name 
                                     then value 
                                     else get_binding_value tl name
;;

(* converts a Name to a variable bound to it, or the given const if it is not a Name*)
let get_data (env: binding list) (n : const) : const =
    match n with
    | Name n -> get_binding_value env n
    | _ -> n
;;

(* perform function/operation 'fn' on top two elements (Ints) of stack *)
let eval_ints (stack : const list) (env : binding list) (fn : int -> int -> int) : const list =
    match stack with
    | n1 :: n2 :: tl -> (match (get_data env n1, get_data env n2) with
                        | (Int x, Int y) -> Int (fn x y) :: tl
                        | _ -> Error :: stack)
    | _ -> Error :: stack
;;

(* perform function/operation 'fn' on top two elements (Ints) of stack, preventing dividing by 0 *)
let eval_ints_no_zero (stack : const list) (env : binding list) (fn : int -> int -> int) : const list =
    match stack with
    | n1 :: n2 :: tl -> (match (get_data env n1, get_data env n2) with
                        | (Int x, Int y) -> if y == 0 
                                            then Error :: stack
                                            else Int (fn x y) :: tl
                        | _ -> Error :: stack)
    | _ -> Error :: stack
;;

(* converts the int at the top of the stack to its opposite *)
let eval_neg (stack : const list) (env : binding list) : const list =
    match stack with
    | n :: tl -> (match get_data env n with
                 | Int x -> Int (0-x) :: tl
                 | _ -> Error :: stack)
    | _ -> Error :: stack
;;

(* swaps the top two elements of the stack *)
let eval_swap (stack : const list) (env : binding list) : const list =
    match stack with
    | x :: y :: tl -> y :: x :: tl
    | _ -> Error :: stack
;;

(* concatenates the top two elements (Strings) of stack *)
let eval_concat (stack : const list) (env : binding list) : const list =
    match stack with
    | n1 :: n2 :: tl -> (match (get_data env n1, get_data env n2) with
                        | (String s1, String s2) -> String (s1 ^ s2) :: tl
                        | _ -> Error :: stack)
    | _ -> Error :: stack
;;

(* perform function/operation 'fn' on top two elements (Bools) of stack *)
let eval_bools (stack : const list) (env : binding list) (fn : bool -> bool -> bool) : const list =
    match stack with
    | n1 :: n2 :: tl -> (match (get_data env n1, get_data env n2) with
                        | (Bool b1, Bool b2) -> Bool (fn b1 b2) :: tl
                        | _ -> Error :: stack)
    | _ -> Error :: stack
;;

(* negates the top element (Bool) of stack *)
let eval_not (stack : const list) (env : binding list) : const list = 
    match stack with
    | n :: tl -> (match get_data env n with
                 | Bool b -> Bool (not b) :: tl
                 | _ -> Error :: stack)
    | _ -> Error :: stack   
;;

(* compares top two elements (Ints) of stack, using function/operation 'fn' *)
let eval_int_compare (stack : const list) (env : binding list) (fn : int -> int -> bool) : const list =
    match stack with
    | n1 :: n2 :: tl -> (match (get_data env n1, get_data env n2) with
                        | (Int x, Int y) -> Bool (fn x y) :: tl
                        | _ -> Error :: stack)
    | _ -> Error :: stack
;;

(* adds a new binding to the environent, or replaces a binding's value if it already exists *)
let rec add_replace_binding (env : binding list) (name : string) (value : const) : binding list =
    match env with
    | [] -> Pair (name, value) :: []
    | Pair (name', value') :: tl -> if name' = name
                                      then Pair (name, value) :: tl
                                      else Pair (name', value') :: add_replace_binding tl name value
;;

(* performs the Bind command, adding a new Name/value pair to the bainding list *)
let eval_bind (stack : const list) (env : binding list) : (const list * binding list) =
    match stack with
    | Name n :: Name v :: tl -> 
        (match get_binding_value env v with
        | Error -> (Error :: stack , env)
        | value -> (Unit :: tl, add_replace_binding env n value)) 
    | Name n :: Error :: tl -> (Error :: stack , env)
    | Name n :: v :: tl -> (Unit :: tl , add_replace_binding env n v)
    | _ -> (Error :: stack , env)
;;

(* performs the If command, pushing values onto the stack based on a bool in the stack *)
let eval_if (stack : const list) (env : binding list) : (const list) =
    match stack with
    | x :: y :: Bool b :: tl -> if b = true
                                then y :: tl
                                else x :: tl
    | _ -> Error :: stack

;;

let eval_fun (fn : string) (inp : string) (cmds : com list) (stack : const list) (env : binding list) : (const list * binding list) = 
    match stack with
    | _ :: tl -> (Unit :: tl, Pair (fn, Funct (inp,cmds)) :: env)
    | _ -> (Error :: stack, env)
;;

let eval_call (stack : const list) (env : binding list) : (const list) =
    match stack with
    | Name n :: Name v :: tl -> 
        (match get_binding_value env v with
        | Error -> (Error :: stack , env)
        | value -> (Unit :: tl, add_replace_binding env n value)) 
    | Name n :: Error :: tl -> (Error :: stack , env)
    | Name n :: v :: tl -> (Unit :: tl , add_replace_binding env n v)
    | _ -> (Error :: stack , env)    
;;

(* evaluates command 'com' using the current stack *)
let eval (cmd : com) (stack : const list) (env : binding list) : (const list * binding list) =
    match cmd with
    | PushI s  -> (convert_int s :: stack , env)
    | PushS s  -> (convert_string s :: stack , env)
    | PushN s  -> (Name s :: stack , env)
    | PushB s  -> (convert_bool s :: stack , env)
    | Push  s  -> (convert_unit s :: stack , env)
    | Pop      -> (eval_pop stack , env)
    | Add      -> (eval_ints stack env ( + ) , env)
    | Sub      -> (eval_ints stack env ( - ) , env)
    | Mul      -> (eval_ints stack env ( * ) , env)
    | Div      -> (eval_ints_no_zero stack env ( / ) , env)
    | Rem      -> (eval_ints_no_zero stack env (fun x -> fun y -> x mod y) , env)
    | Neg      -> (eval_neg stack env , env)
    | Swap     -> (eval_swap stack env , env)
    | Concat   -> (eval_concat stack env , env)
    | And      -> (eval_bools stack env ( && ) , env)
    | Or       -> (eval_bools stack env ( || ) , env)
    | Not      -> (eval_not stack env , env)
    | Equal    -> (eval_int_compare stack env ( = ) , env)
    | LessThan -> (eval_int_compare stack env ( < ) , env)
    | Bind     -> eval_bind stack env
    | If       -> (eval_if stack env, env)
    | Function (fn,inp,cmds) -> eval_fun fn inp cmds stack env
    | Call     -> (eval_call stack env, env)
    | _        -> (stack , env)
;;

(* evaluates the commands in a new environment, returns the remaining command list so the outer environment can continue where this one ends *)
let rec eval_begin (cmds : com list) (stack : const list) (env : binding list) : (com list * const list) =
    match cmds with
    | [] -> ([], stack)
    | End::tl -> (tl, stack)
    | Begin::tl -> let (cmds', stack') = eval_begin tl stack env in (match stack' with
                                                                      | [] -> eval_begin cmds' stack env
                                                                      | hd::tl -> eval_begin cmds' (hd :: stack) env)
    | cmd::tl -> let (new_stack, new_env) = eval cmd stack env in eval_begin tl new_stack new_env
;;

(* evaluate a list of commands, returning the resulting stack *)
let rec eval_cmds (cmds : com list) (stack : const list) (env : binding list) : const list =
    match cmds with
    | [] -> stack
    | Quit::[] -> stack
    | Begin::tl -> let (cmds', stack') = eval_begin tl stack env in (match stack' with
                                                                      | [] -> eval_cmds cmds' stack env
                                                                      | hd::tl -> eval_cmds cmds' (hd :: stack) env)
    | cmd::tl -> let (new_stack, new_env) = eval cmd stack env in eval_cmds tl new_stack new_env
;;



(* --- RESULTS PREPARATION FUNCTIONS *)
(* converts a const to a string based on its type *)
let const_to_string (con : const) : string =
    match con with
    | Int i    -> string_of_int i
    | Bool b   -> "<" ^ string_of_bool b ^ ">"
    | String s -> s
    | Name n   -> n
    | Unit     -> "<unit>"
    | Error    -> "<error>"
    | _        -> ""
;;

(* converts a const list to a string list *)
let rec stack_to_strings (stack : const list) : string list =
    match stack with
    | [] -> []
    | hd::tl -> const_to_string hd :: stack_to_strings tl
;;



(* ===== MAIN INTERPRETER FUNCTION ===== *)
let interpreter (input: string) (output: string) : unit =
    let commands = readfile input in
    let cmd_list = parse_cmds commands in
    let stack    = eval_cmds cmd_list [] [] in
    let printable_stack = stack_to_strings stack in
    writefile output printable_stack
;;
