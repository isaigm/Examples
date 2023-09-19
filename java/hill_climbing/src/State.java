import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class State {

    public State()
    {
        for(int i = 0; i < N; i++)
        {
            int randomRow = (int) (Math.random() * N);
            board.add(new Queen(i, randomRow));
        }
    }
    public State(State other)
    {
        for(int i = 0; i < N; i++)
        {
            int row = other.board.get(i).getRow();
            board.add(new Queen(i, row));
        }
    }
    public ArrayList<State> getNeighbours()
    {
        ArrayList<State> neighbours = new ArrayList<>();
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                if(i != j)
                {
                    State newState = new State(this);
                    newState.board.get(i).setRow(j);
                    neighbours.add(newState);

                }

            }
        }
        return neighbours;
    }
    public boolean isValid()
    {
       return queensAttacked() == 0;
    }
    public int queensAttacked()
    {
        int queensAttacked = 0;
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                if(i != j)
                {
                    var q1 = board.get(i);
                    var q2 = board.get(j);
                    if(attacked(q1, q2)) queensAttacked++;
                }
            }
        }
        return queensAttacked;
    }
    public void print()
    {
        char[] asciiBoard = new char[N * N];
        Arrays.fill(asciiBoard, '.');

        for (Queen queen : board) {
            int row = queen.getRow();
            int col = queen.getCol();
            int idx = col + row * N;
            asciiBoard[idx] = 'Q';
        }
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                char c = asciiBoard[j + i * N];
                System.out.printf("%c   ", c);
            }
            System.out.print("\n");
        }
    }
    private boolean attacked(Queen q1, Queen q2)
    {
        boolean result = q1.getCol() == q2.getCol() || q1.getRow() == q2.getRow();
        int dx = Math.abs(q1.getCol() - q2.getCol());
        int dy = Math.abs(q1.getRow() - q2.getRow());
        result |= (dx == dy);
        return result;
    }
    private final List<Queen> board = new ArrayList<>();
    public static final int N = 50;
}