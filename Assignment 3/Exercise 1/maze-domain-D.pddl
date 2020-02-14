;; Domain definition
(define (domain maze-domain)

    ;; Requirements
    (:requirements :fluents)

    ;; Predicates: Properties of objects that we are interested in (boolean)
    (:predicates
        (RUNNER ?x) ; True if x is a runner (person trying to escape the maze)
        (POSITION ?x) ; True if x is a position
        (OBJECT ?x) ; True if x is an object
        (ROOM ?x) ; True if x is a room
        (AMULET ?x) ; True if x is an amulet
        (runner-at ?x ?y) ; True if runner x is at y
        (object-at ?x ?y) ; True if x is an object, y is a position and x is at y
        (runner-free ?x) ; True if runner x is not carrying anything
        (runner-carry ?x ?y) ; True if runner x is carrying y
        (activated ?x) ; True if room x is activated
        (connected ?x ?y) ; True if position x and y are connected
    )

    ;; WRITE THE FUNCTIONS YOU NEED BASED ON THE maze-problem-D.pddl file

    ;; Actions: Ways of changing the state of the world
    ; The runner r can move from x to y if they are connected
    (:action move
        ; WRITE HERE THE CODE FOR THIS ACTION
        :parameters
        (?r ?x ?y)

        :precondition
        (and
            (RUNNER ?r)
            (POSITION ?x)
            (POSITION ?y)
            (runner-at ?r ?x)
            (not (runner-at ?r ?y))
            (connected ?x ?y)
        )

        :effect
        (and
            (not (runner-at ?r ?x))
            (runner-at ?r ?y)
        )
    )

    ; The runner r can pick up an amulate a in position p if they are not holding anything
    (:action pick-up
        ; WRITE HERE THE CODE FOR THIS ACTION
        ; WHAT DO YOU NEED TO ADD NOW?
        :parameters
        (?r ?p ?a)

        :precondition
        (and
            (RUNNER ?r)
            (POSITION ?p)
            (OBJECT ?a)
            (AMULET ?a)
            (runner-at ?r ?p)
            (object-at ?a ?p)
            (runner-free ?r)
        )

        :effect
        (and
            (not (object-at ?a ?p))
            (not (runner-free ?r))
            (runner-carry ?r ?a)
            (decrease (elements ?p) 1)
        )
    )

    ; The runner r can drop off an amulate a in room p
    (:action drop-off
        ; WRITE HERE THE CODE FOR THIS ACTION
        ; WHAT DO YOU NEED TO ADD NOW?
        :parameters
        (?r ?p ?a)

        :precondition
        (and
            (RUNNER ?r)
            (POSITION ?p)
            (OBJECT ?a)
            (ROOM ?p)
            (AMULET ?a)
            (runner-at ?r ?p)
            (runner-carry ?r ?a)
            (< (elements ?p) 3)
        )

        :effect
        (and
            (object-at ?a ?p)
            (runner-free ?r)
            (not (runner-carry ?r ?a))
            (activated ?p)
            (increase (elements ?p) 1)
        )
    )

)
