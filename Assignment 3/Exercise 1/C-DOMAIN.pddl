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
  (:functions (elements ?position))

  ;; Actions: Ways of changing the state of the world
  ; The runner r can move from x to y if they are connected
  (:action move
    ; WRITE HERE THE CODE FOR THIS ACTION
    ;:parameters(?runner - RUNNER ?x - POSITION ?y - POSITION)
    :parameters(?runner ?x ?y - OBJECT)
    :precondition(and   (RUNNER ?runner)
                        (POSITION ?x)
                        (POSITION ?y)
                        (runner-at ?runner ?x)
                        (connected ?x ?y))
    :effect(and (runner-at ?runner ?y)
                (not (runner-at ?runner ?x)))
  )

  ; The runner r can pick up an amulate a in position p if they are not holding anything
  (:action pick-up
    ; WRITE HERE THE CODE FOR THIS ACTION
    ; WHAT DO YOU NEED TO ADD NOW?
    :parameters(?runner ?position ?obj - OBJECT)
    :precondition ( and
                        (RUNNER ?runner)
                        (POSITION ?position)
                        (AMULET ?obj)
                        (object-at ?obj ?position)
                        (runner-at ?runner ?position)
                        (runner-free ?runner))
    :effect (and
                 (not (object-at ?obj ?position))
                 (not (runner-free ?runner))
                 (runner-carry ?runner ?obj)
                 (decrease (elements ?position) 1))
                 ;(when (<= ?(elements ?room) 3) (not(activated ?room))))

  )

  ; The runner r can drop off an amulate a in room p
  (:action drop-off
    ; WRITE HERE THE CODE FOR THIS ACTION
    ; WHAT DO YOU NEED TO ADD NOW?
    :parameters(?runner ?obj ?room - OBJECT)
    :precondition (and
                        (RUNNER ?runner)
                        (AMULET ?obj)
                        (ROOM ?room)
                        (runner-carry ?runner ?obj)
                        (runner-at ?runner ?room)
                        )
    :effect(and
                    (object-at ?obj ?room)
                    (not (runner-carry ?runner ?obj))
                    (runner-free ?runner)
                    (increase (elements ?room) 1)
                    ;(when (>= ?(elements ?room) 3) (activated ?room)))
                    (activated ?room)
            )
  )

)
