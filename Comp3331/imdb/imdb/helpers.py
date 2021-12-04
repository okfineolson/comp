# COMP3311 20T3 Ass3 ... Python helper functions
# add here any functions to share between Python scripts
import psycopg2




class Connector:
    def __init__(self):
        try:
            self.db = psycopg2.connect("dbname=imdb")
            self.cursor = self.db.cursor()
        except psycopg2.Error as err:
            raise err

    def run(self, sql, params=None):
        try:
            self.cursor.run(sql, params)
        except psycopg2.Error as err:
            raise err
        return self.cursor.fetchall()

    def close(self):
        self.db.close()


def run(sql):
    con = Connector()
    rows = con.run(sql)
    con.close()
    return rows


def handle_regex(title: str) -> str:
    title = title.replace("'", "='").replace("_", "=_")
    if not title.startswith("^"):
        title = "%" + title
    else:
        title = title[1:]
    if not title.endswith("$"):
        title = title + "%"
    else:
        title = title[:-1]
    return title




