import sys
from cli import CLI
from gui import GUI

if __name__ == "__main__":
    def print_help_and_abort(progname):
        sys.exit(f"Usage: {progname} [gui|cli] width height mine-count\n"
                  "1 <= width <= 99\n"
                  "1 <= height <= 99\n"
                  "1 <= mine-count <= width * height")

    try:
        width, height, mine_count = int(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4])

        assert(1 <= width <= 99 and 1 <= height <= 99 and 1 <= mine_count <= width * height
               and sys.argv[1] in ("cli", "gui"))

        if sys.argv[1] == "gui":
            GUI(width, height, mine_count)
        elif sys.argv[1] == "cli":
            CLI(width, height, mine_count)

    except (IndexError, AssertionError):
        print("Execution failed.")
        print_help_and_abort(sys.argv[0])
