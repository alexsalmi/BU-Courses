
type 'a stream = Nil | Cons of 'a * (unit -> 'a stream)




(* PART A *)
let read (s : 'a stream) (default : 'a) : ('a * 'a stream) = 
    match s with
    | Cons (head,tail) -> (head, tail ())
    | Nil -> (default, Nil)
;;

(* PART B *)
let skip (s : 'a stream) (p : 'a -> bool) : 'a stream =
    let rec skip_rec (s: 'a stream) (p: 'a -> bool) : 'a stream =
        match s with
        | Nil -> Nil     
        | Cons (head,tail) -> 
            if p head = true
            then Cons (head, fun () -> skip_rec (tail ()) p)
            else skip_rec (tail ()) p
    in
    skip_rec s p
;;


(* PART C *)
(* merge two streams into one, taking elements alternately *)
let rec mergeS (s : 'a stream) (t : 'a stream) : 'a stream = 
    match (s, t) with
    | (Nil, _) -> Nil
    | (_, Nil) -> Nil
    | (Cons (s_head,s_tail), Cons (t_head,t_tail)) -> Cons (s_head, fun () -> Cons(t_head, fun () -> mergeS (s_tail ()) (t_tail ())))
;;

(* PART D *)
let rec twoseq (s : 'a stream) (t: 'a stream) : 'a stream = 
    match s with
    | Cons (head,tail) -> Cons (head, fun () -> twoseq (tail ()) t)
    | Nil ->
        match t with
        | Cons (head,tail) -> Cons (head, fun () -> twoseq Nil (tail ()))
        | Nil -> Nil
;;

(* PART E *)
let rec dupk (x : 'a) (k : int) (s : 'a stream) : 'a stream = 
    match k with
    | 0 -> s
    | _ -> Cons(x, fun () -> dupk x (k-1) s)
;;

(* PART F *)
let rec repeatk (k : int) (s : 'a stream) : 'a stream =
    let rec rptk (k: int) (count :int) (s: 'a stream) : 'a stream =
        match s with
        | Nil -> Nil
        | Cons (head,tail) -> 
            if count = 0
            then repeatk k (tail ())
            else Cons (head, fun () -> rptk k (count-1) s) 
    in
    rptk k k s
;;


(* PART G *)
let rec addAdjacent (s : int stream) : int stream = 
    match s with
    | Nil -> Nil
    | Cons (head,tail) -> 
        match tail () with
        | Nil -> Nil
        | Cons (head2,tail2) -> Cons (head + head2, fun () -> addAdjacent (tail2 ()))
;;

(* PART H *)

let rec addAdjacentk (k : int) (s : int stream) : int stream =
    let rec addk (k: int) (sum: int) (s: int stream) : (int stream) =
        if k = 0 
        then Cons (sum, fun () -> s)
        else match s with
            | Nil -> Nil
            | Cons (head,tail) -> addk (k-1) (head+sum) (tail ())
    in
    match addk k 0 s with
    | Nil -> Nil
    | Cons(head,tail) -> Cons (head, fun () -> addAdjacentk k (tail()))
;;


(* PART I *)
let rec binOpSeq (f : 'a -> 'b -> 'c) (s : 'a stream) (t : 'b stream) : 'c stream =
    match (s,t) with
    | (Nil,_) -> Nil
    | (_,Nil) -> Nil
    | (Cons (s_head,s_tail), Cons (t_head, t_tail)) -> Cons (f s_head t_head, fun () -> binOpSeq f (s_tail ()) (t_tail ()))
;;

(* PART J *)
let addSeq (ls : int stream)  (rs : int stream) : int stream = 
    binOpSeq ( + ) ls rs
;;

let mulSeq (ls : int stream)  (rs : int stream) : int stream =
    binOpSeq ( * ) ls rs
;;

(* PART K *)
let zipS (s : 'a stream) (t : 'b stream) : ('a * 'b) stream =
    let combine (x: 'a) (y: 'b) : 'c =
        (x,y)
    in
    binOpSeq combine s t
;;

(* PART L *)

let unzipS (s : ('a * 'b) stream) : ('a stream * 'b stream) = 
    let separate_a (x: ('a * 'b)) (y: ('a * 'b)) : 'a = 
        match x with | (a,b) -> a
    in
    let separate_b (x: ('a * 'b)) (y: ('a * 'b)) : 'b = 
        match x with | (a,b) -> b
    in
    let astream = binOpSeq separate_a s s in
    let bstream = binOpSeq separate_b s s in
    (astream, bstream)
;; 


(* Helper functions for testing *)
let rec const (a : 'a) : 'a stream = Cons (a , fun () -> const a);;

let ones = const 1;;

let twos = const 2;;

let greetings = const "hello";;

let rec countFrom (i:int) : int stream = 
  Cons (i, fun () -> countFrom (i+1))
;;

let rec nats : int stream = countFrom 0;;

let rec take (n : int)  (s : 'a stream) : 'a list = 
  if n > 0
  then match s with
        | Nil -> []
        | Cons(head,tail) -> head :: take (n-1) (tail ())
  else []
;;