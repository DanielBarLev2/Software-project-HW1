import math


class Vector:
    def __init__(self, *components):
        self.components = list(components)
        self.cluster = None

    def __repr__(self):
        return f"Vector{tuple(self.components)}"

    def __add__(self, other):
        if len(self.components) != len(other.components):
            raise ValueError("Vectors must have the same dimension for addition")
        summed_components = [x + y for x, y in zip(self.components, other.components)]
        return Vector(*summed_components)

    def __sub__(self, other):
        if len(self.components) != len(other.components):
            raise ValueError("Vectors must have the same dimension for subtraction")
        subtracted_components = [x - y for x, y in zip(self.components, other.components)]
        return Vector(*subtracted_components)

    def __mul__(self, scalar):
        multiplied_components = [x * scalar for x in self.components]
        return Vector(*multiplied_components)

    def __rmul__(self, scalar):
        return self.__mul__(scalar)

    def sum(self):
        return sum(self.components)

    def __pow__(self, power, modulo=None):
        powered_components = [x ** power for x in self.components]
        return Vector(*powered_components)

    def euclidean_distance(self, other):
        return ((self.__sub__(other) ** 2).sum()) ** 0.5
