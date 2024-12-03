# AoC 2024 - syminical

import sys
import traceback



def main():
    # check if the input file was provided
    if not (args := sys.argv[1:]):
        print("\n"
            "Usage: list_distance.py [FILE]\n"
            "Print the distance between the two lists in FILE.\n\n"
            "e.g. FILE:\n"
            "   3  4\n"
            "   9  5\n"
            "   8  7\n")
        sys.exit(0)

    safe = 0
    try:
        with open(args[0], "r") as IN_FILE:
            for row in IN_FILE:
                if not is_row_failed(row):
                    safe += 1
        print(safe)
    except:
        traceback.print_exc()
        print("There was a problem with the input file. Make sure it contains 2 list values per line.")
        sys.exit(1)


def is_row_failed(row):
    levels = row.split()
    for i in range(len(levels)):
        prev = None
        direction = None
        failed = False
        for j, level in enumerate(levels):
            if i == j:
                continue
            current = int(level)
            if prev is None:
                prev = current
                continue

            difference = prev - current
            if abs(difference) > 3 or abs(difference) < 1:
                failed = True
                break
            if direction == None:
                direction = -1 if difference > 0 else 1

            if difference > 0:
                if direction != -1:
                    failed = True
                    break
            elif difference < 0:
                if direction != 1:
                    failed = True
                    break
            prev = current
        if not failed:
            return False
    return True



if __name__ == "__main__":
    main()
