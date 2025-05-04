"""A simple SQLite CLI for the sqlite3 module.

Apart from using 'argparse' for the command-line interface,
this module implements the REPL as a thin wrapper around
the InteractiveConsole class from the 'code' stdlib module.
"""
import sqlite3
import sys

from argparse import ArgumentParser
from code import InteractiveConsole
from contextlib import contextmanager
from textwrap import dedent


def execute(c, sql, suppress_errors=True):
    """Helper that wraps execution of SQL code.

    This is used both by the REPL and by direct execution from the CLI.

    'c' may be a cursor or a connection.
    'sql' is the SQL string to execute.
    """

    try:
        for row in c.execute(sql):
            print(row)
    except sqlite3.Error as e:
        tp = type(e).__name__
        try:
            print(f"{tp} ({e.sqlite_errorname}): {e}", file=sys.stderr)
        except AttributeError:
            print(f"{tp}: {e}", file=sys.stderr)
        if not suppress_errors:
            sys.exit(1)


class SqliteInteractiveConsole(InteractiveConsole):
    """A simple SQLite REPL."""

    def __init__(self, connection):
        super().__init__()
        self._con = connection
        self._cur = connection.cursor()

    def runsource(self, source, filename="<input>", symbol="single"):
        """Override runsource, the core of the InteractiveConsole REPL.

        Return True if more input is needed; buffering is done automatically.
        Return False if input is a complete statement ready for execution.
        """
        match source:
            case ".version":
                print(f"{sqlite3.sqlite_version}")
            case ".help":
                print("Enter SQL code and press enter.")
            case ".quit":
                sys.exit(0)
            case _:
                if not sqlite3.complete_statement(source):
                    return True
                execute(self._cur, source)
        return False


def _complete(text, state):
    keywords = ["ABORT", "ACTION", "ADD", "AFTER", "ALL", "ALTER", "ALWAYS",
                "ANALYZE", "AND", "AS", "ASC", "ATTACH", "AUTOINCREMENT",
                "BEFORE", "BEGIN", "BETWEEN", "BY", "CASCADE", "CASE", "CAST",
                "CHECK", "COLLATE", "COLUMN", "COMMIT", "CONFLICT",
                "CONSTRAINT", "CREATE", "CROSS", "CURRENT", "CURRENT_DATE",
                "CURRENT_TIME", "CURRENT_TIMESTAMP", "DATABASE", "DEFAULT",
                "DEFERRABLE", "DEFERRED", "DELETE", "DESC", "DETACH",
                "DISTINCT", "DO", "DROP", "EACH", "ELSE", "END", "ESCAPE",
                "EXCEPT", "EXCLUDE", "EXCLUSIVE", "EXISTS", "EXPLAIN", "FAIL",
                "FILTER", "FIRST", "FOLLOWING", "FOR", "FOREIGN", "FROM",
                "FULL", "GENERATED", "GLOB", "GROUP", "GROUPS", "HAVING", "IF",
                "IGNORE", "IMMEDIATE", "IN", "INDEX", "INDEXED", "INITIALLY",
                "INNER", "INSERT", "INSTEAD", "INTERSECT", "INTO", "IS",
                "ISNULL", "JOIN", "KEY", "LAST", "LEFT", "LIKE", "LIMIT",
                "MATCH", "MATERIALIZED", "NATURAL", "NO", "NOT", "NOTHING",
                "NOTNULL", "NULL", "NULLS", "OF", "OFFSET", "ON", "OR",
                "ORDER", "OTHERS", "OUTER", "OVER", "PARTITION", "PLAN",
                "PRAGMA", "PRECEDING", "PRIMARY", "QUERY", "RAISE", "RANGE",
                "RECURSIVE", "REFERENCES", "REGEXP", "REINDEX", "RELEASE",
                "RENAME", "REPLACE", "RESTRICT", "RETURNING", "RIGHT",
                "ROLLBACK", "ROW", "ROWS", "SAVEPOINT", "SELECT", "SET",
                "TABLE", "TEMP", "TEMPORARY", "THEN", "TIES", "TO",
                "TRANSACTION", "TRIGGER", "UNBOUNDED", "UNION", "UNIQUE",
                "UPDATE", "USING", "VACUUM", "VALUES", "VIEW", "VIRTUAL",
                "WHEN", "WHERE", "WINDOW", "WITH", "WITHOUT"]
    options = [c + " " for c in keywords if c.startswith(text.upper())]
    try:
        return options[state]
    except IndexError:
        return None

@contextmanager
def _enable_completer():
    try:
        import readline
    except ImportError:
        yield
        return

    old_completer = readline.get_completer()
    try:
        readline.set_completer(_complete)
        if readline.backend == "editline":
            # libedit uses "^I" instead of "tab"
            command_string = "bind ^I rl_complete"
        else:
            command_string = "tab: complete"
        readline.parse_and_bind(command_string)
        yield
    finally:
        readline.set_completer(old_completer)

def main(*args):
    parser = ArgumentParser(
        description="Python sqlite3 CLI",
    )
    parser.add_argument(
        "filename", type=str, default=":memory:", nargs="?",
        help=(
            "SQLite database to open (defaults to ':memory:'). "
            "A new database is created if the file does not previously exist."
        ),
    )
    parser.add_argument(
        "sql", type=str, nargs="?",
        help=(
            "An SQL query to execute. "
            "Any returned rows are printed to stdout."
        ),
    )
    parser.add_argument(
        "-v", "--version", action="version",
        version=f"SQLite version {sqlite3.sqlite_version}",
        help="Print underlying SQLite library version",
    )
    args = parser.parse_args(*args)

    if args.filename == ":memory:":
        db_name = "a transient in-memory database"
    else:
        db_name = repr(args.filename)

    # Prepare REPL banner and prompts.
    if sys.platform == "win32" and "idlelib.run" not in sys.modules:
        eofkey = "CTRL-Z"
    else:
        eofkey = "CTRL-D"
    banner = dedent(f"""
        sqlite3 shell, running on SQLite version {sqlite3.sqlite_version}
        Connected to {db_name}

        Each command will be run using execute() on the cursor.
        Type ".help" for more information; type ".quit" or {eofkey} to quit.
    """).strip()
    sys.ps1 = "sqlite> "
    sys.ps2 = "    ... "

    con = sqlite3.connect(args.filename, isolation_level=None)
    try:
        if args.sql:
            # SQL statement provided on the command-line; execute it directly.
            execute(con, args.sql, suppress_errors=False)
        else:
            # No SQL provided; start the REPL.
            with _enable_completer():
                console = SqliteInteractiveConsole(con)
                console.interact(banner, exitmsg="")
    finally:
        con.close()

    sys.exit(0)


if __name__ == "__main__":
    main(sys.argv[1:])
