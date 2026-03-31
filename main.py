import typer
import random
from typing import Optional


app = typer.Typer()


@app.command()
def main(n: int, seed: Optional[int] = 42):
    random.seed(seed)
    print(n)

    for _ in range(n):
        print("{:.2f}".format(random.random()))
    for _ in range(n):
        print("{:.2f}".format(random.random()))



if __name__ == "__main__":
    app()
