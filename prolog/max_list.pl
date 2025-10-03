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


maxList_([X | Xs], Res) :-
    maxList_tr(Xs, X, Res).

maxList_tr([], Acc, Acc).
maxList_tr([X | Xs], Acc, Res) :- 
    max(X, Acc, Res_),
    maxList_tr(Xs, Res_, Res).
