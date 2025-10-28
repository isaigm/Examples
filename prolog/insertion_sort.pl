max(A, B, A) :- A > B.

max(A, B, B) :- A =< B.

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


insertion_sort(L1, Res) :-
    insertion_sort_tr(L1, [], Res).

insertion_sort_tr([], Acc, Acc).

insertion_sort_tr(L, Acc, Res) :-
    maxList(L, E),
    select(E, L, L_),
    Acc_ = [E | Acc],
    !,
    insertion_sort_tr(L_, Acc_, Res).
