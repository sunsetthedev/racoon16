#!/usr/bin/env python3
import sys


def main():
    if len(sys.argv) < 2:
        print("Usage: txt2bin.py <input.txt> [output.bin]", file=sys.stderr)
        sys.exit(1)

    infile = sys.argv[1]
    outfile = sys.argv[2] if len(sys.argv) > 2 else infile.rsplit(".", 1)[0] + ".bin"

    with open(infile, "r") as f:
        text = f.read().replace("\n", " ").strip()

    data = bytes.fromhex(text)

    with open(outfile, "wb") as f:
        f.write(data)

    print(f"Wrote {len(data)} bytes to {outfile}")


if __name__ == "__main__":
    main()