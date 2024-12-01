# AoC 2024 - 12/1/24 03:23
# syminical

import sys



def main():
    # check if the input file was provided
    if not (args := sys.argv[1:]):
        print("\n"
            "Usage: list_similarity.py [FILE]\n"
            "Print the similarity score for the two lists in FILE.\n\n"
            "e.g. FILE:\n"
            "   3  4\n"
            "   9  5")
        sys.exit(0)

    # try to read the input file into two lists
    list_a = []
    list_b = {}
    try:
        with open(args[0], "r") as IN_FILE:
            for row in IN_FILE:
                list_values = row.split()
                list_a.append(int(list_values[0]))

                # keep track of how many times a value is present in list_b
                if (ib := list_values[1]) in list_b:
                    list_b[ib] += 1
                else:
                    list_b[ib] = 1
    except:
        print("There was a problem with the input file. Make sure it contains 2 list values per line.")

    # find the similarity scores for values in list_a
    list_similarities = [i * list_b.get(str(i), 0) for i in list_a]

    # print the sum
    print(sum(list_similarities))



if __name__ == '__main__':
    main()
