max(A, B, Res) :- A > B,
    Res = A.

max(A, B, Res) :- A < B,
    Res = B.

max(A, B, Res) :- A =:= B,
    Res = A.

maxList([X], X).
maxList([X | XS], Res) :-
    maxList(XS, Res_),
    max(Res_, X, Res).
