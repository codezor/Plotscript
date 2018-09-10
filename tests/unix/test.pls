; this is a comment, ignore any text after ; to end of the line

; ----------------------------------------------------------------------------
; begin is a special form that executes multiple expressions in order
( begin

; ----------------------------------------------------------------------------
; the simplest expression is a single literal atom
(1)
(18.562)
(pi) ; this is a symbol in the default environment

; ----------------------------------------------------------------------------
; define creates and binds a symbol
; in the simplest case this binds to a literal atom
(define a 1)
(define b 12)

; ----------------------------------------------------------------------------
; a more complex expression involves a procedure
(+ 1 2)
(+ I 3)
;
; which in some cases may have more than 2 arguments (m-ary)
(+ 1 2 3 4 5 6)
(+ I 3 5 6)
; some cases have comple numbers
(* I 4 5 6)
(* I I I)
; and in other cases can have only two (binary)
(- 2 1)
(/ 3 I)
;
(/ I I)
(/ I 3)
;
(- 2 I)
(- I 1)
;
(- I I)
(- I 0)
;
(^ 3 4)
(^ I 3)
;
(^ 3 I)
(^ I I)
;but in some cases may have one (unary)
(sqrt 4)
;
(sqrt -4)
;
(sqrt I)
;
(ln 4)
;
(sin 3)
;
(cos 5)
;
(tan 4)
;
(real I)
;
(imag I)
;
(mag I)
;
(arg I)
;
(conj I)
;
(- I)
;
(- 4)
; there are a several built-in procedures (see specification)


) ; end begin
