import colorama, argparse, os, sys
from pathlib import Path 
SPACE = 4
colorama.init(autoreset=True)
def print_tree(path, is_end, prefix):
    level = ""
    if is_end:
        level = prefix + SPACE * ' ' + "└──" + (colorama.Fore.BLUE + path.name if path.is_dir() else path.name)
    else:
        level = prefix + SPACE * ' ' + "├──" + (colorama.Fore.BLUE + path.name if path.is_dir() else path.name)
    print(level)

    if not path.is_dir():
        return 
    
    iters = [x for x in path.iterdir()]
    for i in range(len(iters)):
        if is_end:
            print_tree(iters[i], i == len(iters) - 1, prefix + SPACE * ' ')
        else:
            print_tree(iters[i], i == len(iters) - 1, prefix + SPACE * ' ' + '│')
                

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Print tree view of a given directory")
    parser.add_argument('Path',
                       metavar='path',
                       type=str,
                       help='path of the directory')
    args = parser.parse_args()

    input_path = args.Path
    
    if not os.path.isdir(input_path):
        print('The path specified does not exist')
        sys.exit()
    
    print_tree(Path(input_path), True, "")

