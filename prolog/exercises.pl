:- use_module(library(clpfd)).

member(E, [X | _]) :- E = X.
member(E, [_ | Xs]) :- member(E, Xs).

subconjunto([], _).
subconjunto([X | Xs], L) :- 
    member(X, L),
    subconjunto(Xs, L).

inter(L1, L2, Res) :-
    inter_tr(L1, L2, [], Res_),
    reverse(Res_, Res).
inter_tr([], _, Acc, Acc).
inter_tr([X | Xs], L, Acc, Res) :-
    (
        member(X, L)
        -> Acc_ = [X | Acc]
        ; Acc_ = Acc
    ),
    inter_tr(Xs, L, Acc_, Res).


dif(L1, L2, Res) :-
    dif_tr(L1, L2, [], Res_),
    reverse(Res_, Res).
dif_tr([], _, Acc, Acc).
dif_tr([X | Xs], L, Acc, Res) :-
    (
        member(X, L)
        -> Acc_ = Acc
        ; Acc_ = [X | Acc]
    ),
    dif_tr(Xs, L, Acc_, Res).

union(L1, L2, Res) :- 
    append(L1, L2, Res_),
    list_to_set(Res_, Res).


select(E, [E | T], T).
select(E, [H | T], [H | Resto]) :-
    select(E, T, Resto).


permutation([], []).
permutation(L, [H | T]) :-
    select(H, L, Resto),      
    permutation(Resto, T).   


peanoToNat(0, 0).
peanoToNat(s(P), N) :-
    N #> 0,
    N #= N_prev + 1,
    peanoToNat(P, N_prev).

sumaPeano(N1, N2, Res) :-
    peanoToNat(N1, N1_),
    peanoToNat(N2, N2_),
    Sum #= N1_ + N2_,
    peanoToNat(Res, Sum).

restaPeano(N1, N2, Res) :-
    peanoToNat(N1, N1_),
    peanoToNat(N2, N2_),
    Resta #= N1_ - N2_,
    Resta #>= 0,
    peanoToNat(Res, Resta).
