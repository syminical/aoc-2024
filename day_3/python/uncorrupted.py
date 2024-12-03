# AoC 2024 - syminical

import re
import sys
import traceback



def main():
    # check if the input file was provided
    if not (args := sys.argv[1:]):
        print("\n"
            "Usage: uncorrupted.py [FILE]\n"
            "Print the distance between the two lists in FILE.\n\n"
            "e.g. FILE:\n"
            "   3  4\n"
            "   9  5\n"
            "   8  7\n")
        sys.exit(0)

    instr_regex = "(?:mul\((?P<A>\d+),(?P<B>\d+)\))"

    sum = 0
    try:
        with open(args[0], "r") as IN_FILE:
            rows = IN_FILE.read()
            while (i := rows.find("don't()")) >= 0:
                print('workin')
                if (j := rows.find("do()", i)) > i:
                    rows = rows.replace(rows[i:j], "")
                else:
                    rows = rows.replace(rows[i:-1], "")

            for row in rows.split():
                for a,b in re.findall(instr_regex, row):
                    sum += int(a) * int(b)
        print(sum)
    except:
        traceback.print_exc()
        print("There was a problem with the input file. Make sure it contains 2 list values per line.")
        sys.exit(1)



if __name__ == "__main__":
    main()
