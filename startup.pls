(begin
(define make-point(lambda (x y) (set-property "size" 0 (set-property "object-name" "point" (list x y)))))
;(define p1 (lambda(x y) (make-point x y)))
;(define p2 (lambda(x y) (make-point x y)))

(define make-line (lambda (p1 p2) (set-property "thickness" 1 (set-property "object-name" "line" (list (define p1 (lambda(x y) (make-point x y))))(define p2 (lambda(x y) (make-point x y)))))))
(define make-text (lambda (str) ( set-property "position" (define origin (make-point 0 0)) (set-property "object-name" "text" (list str )))))
)
