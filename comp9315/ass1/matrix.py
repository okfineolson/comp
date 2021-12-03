class Matrix:
    def __init__(self, m, n):
        """Initialises (with zeros) a matrix of dimensions m by n."""
        self.row = m
        self.col = n
        self.mat = [[0 for i in range(self.col)] for i in range(self.row)]
    def __str__(self):
        """Returns a string representation of this matrix as integers in the form:
          a b c
          d e f
          g h i
        Used as follows: s = str(m1)
        """
        return self.mat

    def get(self, key):
        """Returns the (i,j)th entry of the matrix, where key is the tuple (i, j)

        Used as follows: x = matrix.get((0,0))
        * raises IndexError if (i,j) is out of bounds
        """
               
        if key[0] >= self.row or key[1] >= self.col:
           raise IndexError("(i, j) is out of bounds")
        else:
             return self.mat[key[0]][key[1]]

    def set(self, key, data):
        """Sets the (i,j)th entry of the matrix, where key is the tuple (i, j)

        and data is the number being added.
        Used as follows: matrix.set((0,0), 1)
        * raises IndexError if (i,j) is out of bounds
        * raises TypeError if data is not an integer
        """
        if key[0] >= self.row or key[1] >= self.col:
           raise IndexError("(i, j) is out of bounds")
        elif type(data) != int:
             raise TypeError("data not a integer")
        else:
             self.mat[key[0]][key[1]] = data

    def add(self, other):
        """Adds self to another Matrix or integer, returning a new Matrix.

        This method should not modify the current matrix or other.
        Used as follows: m1.add(m2) => m1 + m2
        or: m1.add(3) => m1 + 3
        * raises TypeError if other is not a Matrix object or an integer
        * raises ValueError if the other Matrix does not have the same dimensions
        """
        
        if type(other) != int or type(other) != str:
           raise TypeError("other is not a Matrix object or an integer")
        elif type(other) == str:
             if self.row != other.row or self.col != other.col:
                raise ValueError("the other Matrix does not have the same dimensions")
             else:
                 self.val = set((self.i,self.j), list(self.val)[1]+list(other.val)[1])
        elif type(other) == int:
             self.val = set((self.i,self.j), list(self.val)[1]+other)

    def mul(self, other):
        """Multiplies self with another Matrix or integer, returning a new Matrix.

        This method should not modify the current matrix or other.
        Used as follows: m1.mul(m2) m1 x m2 (matrix multiplication, not point-wise)
        or: m1.mul(3) => m1*3
        * raises TypeError if the other is not a Matrix object or an integer
        * raises ValueError if the other Matrix has incorrect dimensions
        """
        
        if type(other) != int or type(other) != str:
           raise TypeError("other is not a Matrix object or an integer")
        elif type(other) == str:
             if self.col != other.row:
                raise ValueError("the other Matrix has incorrect dimensions")
             else:
                 for loop in self.col:
                     if self.i == other.j and self.j == other.i:
                        new_val += list(self.val)[1]*list(other.val)[1]
                 self.val = set((self.i,self.j), new_data)
        elif type(other) == int:
             self.val = set((self.i,self.j),list(self.val)[1]*other)
             
             
             
             
             
             
             
