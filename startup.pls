(begin
(define make-point(lambda (x y) (list x y)))
(define make-point (set-property "object-name" "point" make-point))
(define make-point (set-property  "size" 0 make-point))

;define p1 (lambda (p1x p1y) (make-point p1x p1y)))
;define p2 (lambda (p2x p2y) (make-point p2x p2y)))
(define make-line (lambda (p1 p2) (list(define p1 (lambda (p1x p1y) (make-point p1x p1y)))(define p2 (lambda (p2x p2y) (make-point p2x p2y))))))
(define make-line (set-property "object-name" "line" make-line ))
(define make-line(set-property "thickness" 1  make-line))

(define make-text (lambda ( str) (list str ) ))
(define make-text(set-property "object-name" "text" make-text))
(define origin (list 0 0))
(define make-text(set-property "position"origin make-text))
)
