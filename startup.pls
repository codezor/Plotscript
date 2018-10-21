(begin
(define make-point (lambda (x y) (set-property "size" 0 (set-property "object-name" "point" (list x y)))))
;(define p1 (make-point))
;(define p2 (make-point))
;(lambda (p1 p2)(list (define p1(lambda(x1 y1)(make-point x1 y1 )))(define p2(lambda(x2 y2)(make-point x2 y2)))))
; (define make-line (lambda (p1 p2) (list p1 p2 )))
;(define make-line (lambda (p1 p2) (set-property "thickness" 1 (set-property "object-name" "line" (list p1  p2)))))
(define make-line (lambda (p1 p2) (set-property "thickness" 1 (set-property "object-name" "line" (list p1 p2)))))
(define make-text (lambda (str) ( set-property "position" (define origin (make-point 0 0)) (set-property "object-name" "text" (str)))))

;(define a (make-line (make-point 0 0) (make-point 20 20)))
)