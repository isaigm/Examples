package det.a;
import java.math.*;
import java.util.Scanner;
public class DetA {
    public static void main(String[] args) {
        int ord = 2;
        int matriz [][] = new int[ord][ord];
        Scanner read = new Scanner(System.in);
        for(int i = 0; i < ord; i++){
            for(int j = 0; j < ord; j++){
                matriz[i][j] = read.nextInt();
            }
        }
       System.out.println(det(matriz, ord));
       impr(matriz, ord);
    }
    static void impr(int matriz[][], int ord){
         for(int i = 0; i < ord; i++){
            for(int j = 0; j < ord; j++){
                System.out.printf("%d ", matriz[i][j]);
            }
            System.out.println();
        }
    }
    static int [][] inicioSub(int matriz[][], , int col, int orden){
        int resultado[][]  = new int[orden - 1][orden - 1];
        int k = 0, l = 0;
        for(int i = 0; i < orden; i++){
            for(int j = col; j < orden; j++){
                if(i != fila && j != col){
                    resultado[k][l] = matriz[i][j];
                    l++;
                    if(l >= orden - 1){
                        k++;
                        l = 0;
                    }
                }
            }
        }
        return resultado;
    }
    static int det(int matriz[][], int orden){
        int det = 0;
        if(orden == 1) return matriz[0][0];

        for(int i = 0; i < orden; i++){
            int aux[][] = inicioSub(matriz, 0, i, orden);
            //impr(aux, orden - 1);
            det =  det +  (int) Math.pow(-1, i) * matriz[0][i] * det(aux, orden - 1);
        }
            
        
        return det;
    }
}
