#include <stdio.h>
#include <string>
#include <iostream>
#include <map>
static uint8_t *ptr;
void decode(const std::string &program, int i)
{
    switch (program[i])
    {
    case '>':
        ++ptr;
        break;
    case '<':
        --ptr;
        break;
    case '-':
        --(*ptr);
        break;
    case '+':
        ++(*ptr);
        break;
    case '.':
        putchar(*ptr);
        break;
    case ',':
        *ptr = getchar();
        break;
    default:
        break;
    }
}
void helper(const std::string &program, std::map<int, int> &brackets, size_t &i){
    size_t end_loop = brackets[i];
    size_t start_loop = i + 1;
    while (*ptr != 0)
    {
        for(i = start_loop; i < end_loop; i++){
            decode(program, i);
            if(program[i] == '['){
                helper(program, brackets, i);
            }
        }
    }
    i = end_loop;
}
void interpreter(const std::string &program, std::map<int, int> &brackets, size_t &i)
{
    while (i < program.size())
    {
        decode(program, i);
        if(program[i] == '['){
            helper(program, brackets, i);
        }
        i++;
    }
}
int find_closing_bracket(const std::string &program, int pos){
     int depth = 1;
     for (int i = pos + 1; i < int(program.length()); i++) {
         switch (program[i]) {
       case '[':
         depth++;
         break;
       case ']':
         if (--depth == 0) {
           return i;
         }
         break;
       }
     }
     return -1;
}
int main()
{
    const std::string program = ">++++[<++++++++>-]>++++++++[>++++<-]>>++>>>+>>>+<<<<<<<<<<[-[->+<]>[-<+>>>.<<]>>>[[->++++++++[>++++<-]>.<<[->+<]+>[->++++++++++<<+>]>.[-]>]]+<<<[-[->+<]+>[-<+>>>-[->+<]++>[-<->]<<<]<<<<]++++++++++.+++.[-]<]+++++";
    uint8_t mem[0x7530] = {0};
    std::map<int, int> brackets;
    ptr = &mem[0];
    size_t i = 0;
    for(size_t j = 0; j < program.size(); j++){
        if(program[j] == '['){
            int c = find_closing_bracket(program, j);
            if(c == -1) exit(1);
            brackets[j] = c;
        }
    }
    interpreter(program, brackets, i);
    return 0;
}
