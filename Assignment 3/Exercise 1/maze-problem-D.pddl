;; Problem definition
(define (problem problem-1)

  ;; Specifying the domain for the problem
  (:domain maze-domain)

  ;; Objects definition
  (:objects
    ; Positions
    entrance
    hall
    room-A room-B
    ; Amulets
    amulet-1 amulet-2 amulet-3 amulet-4 amulet-5 amulet-6
    ; Runners
    jana
  )

  ;; Initial state of problem 1
  (:init
    ;; Declaration of the objects
    ; Runners
    (RUNNER jana)
    ; Positions
    (POSITION entrance) (POSITION hall)
    (POSITION room-A) (POSITION room-B)
    ; Rooms
    (ROOM room-A) (ROOM room-B)
    ; Objects
    (OBJECT amulet-1) (OBJECT amulet-2) (OBJECT amulet-3)
    (OBJECT amulet-4) (OBJECT amulet-5) (OBJECT amulet-6)
    ; Amulets
    (AMULET amulet-1) (AMULET amulet-2) (AMULET amulet-3)
    (AMULET amulet-4) (AMULET amulet-5) (AMULET amulet-6)

    ;; Declaration of the predicates of the objects
    ; We set the runner free
    (runner-free jana)
    ; We assume both rooms are not activated
    ; -> No need to express false predicates
    ; We set the runners in the entrance
    (runner-at jana entrance)
    ; We set all the objects in the hall
    (object-at amulet-1 hall) (object-at amulet-2 hall) (object-at amulet-3 hall)
    (object-at amulet-4 hall) (object-at amulet-5 hall) (object-at amulet-6 hall)
    ; We define the connections between the positions
    (connected room-A hall) (connected hall room-A)
    (connected room-B hall) (connected hall room-B)
    (connected entrance hall) ;; Not bidirectional, we cannot go back

    ;; Declaration of fluents
    ; We set the number of moves of each runner to 0 and the room counters to 0
    (= (elements room-A) 0)
    (= (elements room-B) 0)
    (= (elements hall) 6)

  )

  (:goal
    (and
      ; We want all the rooms' buttons activated
      (activated room-A)
      (activated room-B)
      ;; WRITE WHAT DO YOU NEED TO ADD TO ACCOUNT FOR AT LEAST 3 ELEMENTS PER ROOM
      (>= (elements room-A) 3)
      (>= (elements room-B) 3)
      (= (elements hall) 0)
    )
  )

)
