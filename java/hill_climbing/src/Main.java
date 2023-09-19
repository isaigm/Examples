public class Main {
    public static void main(String[] args) {
        State currState = new State();
        int last = 0;
        while(!currState.isValid())
        {
            var neighbours = currState.getNeighbours();
            int min = neighbours.get(0).queensAttacked();
            int idx = 0;
            for(int i = 1; i < neighbours.size(); i++)
            {
                int h = neighbours.get(i).queensAttacked();
                if(h < min )
                {
                    idx = i;
                    min = h;
                }
            }
            if(min == last || min > last) currState = new State();
            else currState = neighbours.get(idx);
            last = min;
        }
        currState.print();

    }

}