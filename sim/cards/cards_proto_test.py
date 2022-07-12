import unittest

from sim.cards.card_pb2 import Cards
from google.protobuf import text_format

class CardsProtoTest(unittest.TestCase):
    def test_ironclad_load(self):
        with open('sim/cards/ironclad.pbtxt') as file_in:
            c = Cards()
            text_format.Parse(file_in.read(), c)

        self.assertTrue(True)

if __name__ == "__main__":
    unittest.main()

