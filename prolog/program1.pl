% Caso base: 1 es un número par.
punto(0, 1).
par(s(s(0))).

par(s(s(X))) :- write(X),write('\n'), par(X) .

suma(A, B, C) :- C is A + B.
lista(M, N) :- M<N, nl, write(M), NuevoM is M + 1, lista(NuevoM, N).

hola :- write('hola mundo').

sobre(a, b).
sobre(d, c).
sobre(e, d).
libre(a).
libre(e).
ocupado(X) :- sobre(Y, X).
base(X, Y) :- sobre(Y, X).

arista(3, 6).
arista(6, 7).
arista(7, 8).
arista(3, 5).
arista(3, 4).
arista(3, 2).
arista(2, 1).
arista(9, 6).
arista(8, 10).
camino(X, Y) :- arista(X, Y).
camino(X, Y) :- arista(X, Z), camino(Z, Y).

print_list([]).
print_list([H|T]) :- write(H), nl, print_list(T).
