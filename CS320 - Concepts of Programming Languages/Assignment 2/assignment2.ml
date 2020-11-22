
(* PROBLEM 1 *) 

let rec tetra1 (n : int) : int = 
    match n with
    | 0 -> 0
    | 1 -> 1
    | 2 -> 1
    | 3 -> 2
    | x -> tetra1 (x-1) + tetra1 (x-2) + tetra1 (x-3) + tetra1 (x-4)
;;
(* PROBLEM 2 *)

(*
   PART A: This function works as specified on lists of
   length greater than 4.
*)

let sum_top_4 (xs : int list) : int list =
    match List.length xs with
    | 0 -> 0 :: xs
    | 1 -> List.nth xs 0 :: xs
    | 2 -> List.nth xs 0 + List.nth xs 1 :: xs
    | 3 -> List.nth xs 0 + List.nth xs 1 + List.nth xs 2 :: xs
    | _ -> List.nth xs 0 + List.nth xs 1 + List.nth xs 2 + List.nth xs 3 :: xs
;;
(* PART B *)

let ascending (n : int) : int list =
    let rec loop (i : int) : int list =
        if i = n 
        then i :: []
        else i :: loop (i+1)
    in loop 0
;;

(* PART C *)

let rec tetra2 (n : int) : int = 
    let rec map (f : int -> int) (xs : int list) : int list = 
        match xs with
        | [] -> []
        | head :: tail -> f head :: map f tail
    in
    match n with
    | 0 -> 0
    | 1 -> 1
    | 2 -> 1
    | 3 -> 2
    | x -> 
        let ls = ascending (x-1) in
        let lsmapped = map tetra2 ls in
        let lsrev = List.rev lsmapped in
        let sumtop4 = sum_top_4 lsrev in
        List.nth sumtop4 0
;;
(* PROBLEM 3 *)


let rec sum_top_k (k : int) (xs : int list) : int =
    match xs with
    | [] -> 0
    | head :: tail -> if k = 0 then 0 else head + sum_top_k (k-1) tail
;;


let rec fib_k_step (k : int) (n : int) : int = 
    let rec map (f : int -> int) (xs : int list) : int list =
        match xs with
        | [] -> []
        | head :: tail -> f head :: map f tail
    in
    match n with
    | 0 -> 0
    | 1 -> 1
    | 2 -> 1
    | x -> 
        let ls = ascending (x-1) in
        let lsmapped = map (fib_k_step k) ls in
        let lsrev = List.rev lsmapped in
        sum_top_k k lsrev
;;


(* PROBLEM 4 *)

type 'a binTree =
   | Leaf
   | Node of 'a * ('a binTree) * ('a binTree)
;;

let rec mapT (f : 'a -> 'b) (t : 'a binTree) : 'b binTree =
    match t with
    | Leaf -> Leaf
    | Node (x,a,b) -> Node (f x, mapT f a, mapT f b) 
;;

let rec foldT (f: 'a -> 'b -> 'b -> 'b) (t: 'a binTree) (base: 'b) : 'b =
    match t with
    | Leaf -> base
    | Node (x,a,b) -> f x (foldT f a base) (foldT f b base)
;;

(* PROBLEM 5 *) 

let leafCount (t : 'a binTree) : int = 
    let countleafs (head: 'a) (left: 'b) (right: 'b) : 'b =
        left + right
    in
    foldT countleafs t 1
;;

let nodeCount (t : 'a binTree) : int =
    let countnodes (head: 'a) (left: 'b) (right: 'b) : 'b =
        left + right + 1
    in
    foldT countnodes t 0
;;

let height (t : 'a binTree) : int = 
    let countheight (head: 'a) (left: 'b) (right: 'b) : 'b =
        if left > right then left + 1 else right + 1
    in
    foldT countheight t 0
;;

(* PROBLEM 6 *) 

let rec perfect (t : 'a binTree) : bool = 
    match t with
    | Leaf -> true
    | Node (x,a,b) -> 
        if (height a) != (height b)
        then false
        else (perfect a) && (perfect b)
;;

let rec degenerate (t : 'a binTree) : bool = 
    (height t) = (nodeCount t)
;;

let treeToList (t : 'a binTree) : 'a list option = 
    let toArray (head: 'a) (left: 'b) (right: 'b): 'b =
        match (left,right) with
        | ( x,[]) -> head :: x
        | ([],y ) -> head :: y
        | ( _,_ ) -> head :: []
    in
    match degenerate t with
    | false -> None
    | true -> Some (foldT toArray t [])
;;

