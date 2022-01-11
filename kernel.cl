__kernel void perform_step(__global unsigned char *cells, __global unsigned char *tempCells, int rows, int cols)

{
    int row = get_global_id(0);
    int col = get_global_id(1);
    if(row >= rows || col >= cols) return;
  
    int alive_neighbours = 0;

    for(int i = row - 1; i <= row + 1; i++)
    {
        for(int j = col - 1; j <= col + 1; j++)
        {
            if(i < 0 || j < 0 || j >= cols || i >= rows || (i == row && j == col)) continue;
            alive_neighbours += cells[j + i * cols];
        }
    }


    if(cells[col + row * cols])
    {
        if(alive_neighbours == 2 || alive_neighbours == 3)
        {
            tempCells[col + row * cols] = 1;
        }else
        {
            tempCells[col + row * cols] = 0;
        }
       
        
    }else
    {
        tempCells[col + row * cols] = alive_neighbours == 3;
    }
}
