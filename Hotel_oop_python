
class Minibar:
    def __init__(self, drinks={}, snacks={}):
        self.drinks = {key.lower(): value for key,value in drinks.items()}
        self.snacks = {key.lower():value for key, value in snacks.items()}
        self.bill= 0

    def __repr__(self):
        drinks_list = ("Drinks: "+', '.join(self.drinks.keys())) if self.drinks!={} else 'No drinks left'
        snacks_list = ('Snacks: '+', '.join(self.snacks.keys()) if self.snacks!={} else 'No snacks left')
        bill_info = f"\nBill: {self.bill}" if self.bill != 0 else '\nNo bill yet'


        return f"{drinks_list}\n{snacks_list} {bill_info}"

    def eat(self, snack):
        snack = snack.lower()
        if snack in self.snacks:
            price_of_snack=self.snacks.pop(snack)
            self.bill+=price_of_snack
        else:
             print("The snack",snack,"was not found.")
    def drink(self, drink):
        drink=drink.lower()
        if drink in self.drinks and self.drinks[drink]>1:
            self.drinks[drink]-=1
            self.bill+=20
        elif  drink in self.drinks and self.drinks[drink]==1:
            self.bill+=20
            self.drinks.pop(drink)
        else:
            print('The drink',drink,'was not found.')


#########################################
class Room:
    def __init__(self, minibar, number, guests, clean_level, is_suite, satisfaction=0.5):
        self.minibar = minibar
        self.number = number
        self.guests = [guest.lower() for guest in guests]
        if type(clean_level) != int:
            print("TypeError")
        self.clean_level = clean_level
        if type(is_suite) != bool:
            print("TypeError")
        self.is_suite = is_suite
        if satisfaction==1 or satisfaction==0:
            satisfaction=float(satisfaction)
        if type(satisfaction) != float:
            print("TypeError")
        self.satisfaction = satisfaction

    def __repr__(self):
       guest_lst = ", ".join(sorted(self.guests)) if self.guests!=[] else "empty"
       return f'Room number: {self.number} \nGuests: {guest_lst} \nClean level: {self.clean_level} \nIs suite: {self.is_suite} \nSatisfaction: {self.satisfaction} \nMinibar:\n{self.minibar}'


    def is_occupied(self):
        if self.guests==[]:
            return False
        else:
            return True
    def clean(self):
        if self.is_suite==True:
            self.clean_level=min(15,self.clean_level+1+1)
        else:
            self.clean_level=min(15,self.clean_level+1)

    def better_than(self, other):
        if self.is_suite == True and other.is_suite == False:
            return True
        if self.is_suite == True and other.is_suite == True:
            if self.number//100 > other.number//100:
                return True
        if self.number//100 == other.number//100:
            if self.clean_level > other.clean_level:
                return True
            else:
                return False
        else:
            return False

    def check_in(self, guests):
        guests=[gst.lower() for gst in guests]
        if self.guests==[]:
             self.guests=guests
             self.satisfaction=0.5
        else:
            return print("Can't check-in new guests to an occupied room.")

    def check_out(self):
        if self.guests==[]:
            return print("Cannot check-out an empty room.")
        else:
            self.guests=[]
            self.minibar.bill=0

    def move_to(self, other):
        if self.guests==[] :
            return print("Cannot move guests from an empty room")
        if other.is_occupied == True :
            return print("Can't move guests into an occupied room")
        else:
            other.check_in(self.guests)
            other.minibar.bill = self.minibar.bill
            self.check_out()
            if other.better_than(self) == True:
                other.satisfaction = min(1.0, self.satisfaction+0.1)
            else:
                other.satisfaction = self.satisfaction
            self.guests =[]


#########################################
class Hotel:
    def __init__(self, name, rooms):
        self.name = name
        self.rooms = rooms

    def __repr__(self):
        total_num_of_rooms = len(self.rooms)
        number_of_occ = 0
        for room in self.rooms:
            if room.is_occupied() == True:
                number_of_occ +=1
            else:
                continue
        return f'{self.name} Hotel has: {number_of_occ}/{total_num_of_rooms} occupied rooms.'

    def check_in(self, guests, is_suite):
        if is_suite==True:
            for room in self.rooms:
                if room.is_suite == True and room.is_occupied() == False:
                    room.check_in(guests)
                    return room
        if is_suite == False:
            for room in self.rooms:
                if room.is_suite == False and room.is_occupied() == False:
                    room.check_in(guests)
                    return room
        else:
            return None

    def check_out(self, guest):
        guest = guest.lower()
        for room in self.rooms:
            if guest in room.guests:
                room.check_out()
                return room
        return None

    def upgrade(self, guest):
        guest = guest.lower()
        current_room = None
        for room in self.rooms:
            if guest in room.guests:
                current_room=room
                break
        if current_room == None:
            return current_room
        for new_room in self.rooms:
            if current_room.better_than(new_room) == False and new_room.is_occupied() == False:
                current_room.move_to(new_room)
                return new_room


    def send_cleaner(self, guest):
        guest = guest.lower()
        target_room = None
        for room in self.rooms:
            if guest in room.guests:
                target_room = room
        if target_room == None:

            return target_room
        else:
            target_room.clean()
            return target_room
############################
#############TESTS##########
############################



if __name__ == '__main__':
    def test_hotel():
        m1 = Minibar({'coke': 10, 'lemonade': 1}, {'bamba': 8, 'mars': 12})
        m2 = Minibar({'beer': 10, 'lemonade':4}, {'m&m': 6})
        rooms = [Room(m2, 514, [], 5, True),
                 Room(m2, 210, ["Ronen", "Shir"], 6, True),
                 Room(m1, 102, ["Liat"], 7, False),
                 Room(m2, 223, [], 6, True)]
        h = Hotel("Dan", rooms)
        test_sep = '\n------------------'
        print('PRINT m1:\n', m1, test_sep, sep="")
        print('PRINT m2:\n', m2, test_sep, sep="")
        print('PRINT h:\n', h, test_sep, sep="")
        liat_room = h.send_cleaner('Liat')
        print('CALL: h.send_cleaner("Liat")\n', liat_room, test_sep, sep="")
        print('CALL: liat_room.eat("bamba")\n', liat_room.minibar.eat("bamba"), test_sep, sep="")
        print('PRINT liat_room.minibar:\n', liat_room.minibar, test_sep, sep="")
        print('CALL: liat_room.drink("lemonade")\n', liat_room.minibar.drink("lemonade"), test_sep, sep="")
        print('PRINT liat_room.minibar:\n', liat_room.minibar, test_sep, sep="")
        print('CALL: h.upgrade("Liat")\n', h.upgrade("Liat"), test_sep, sep="")

        print('CALL: h.check_out("Ronen")\n', h.check_out("Ronen"), test_sep, sep="")
        print('CALL: h.check_out("Ronen")\n', h.check_out("Ronen"), test_sep, sep="")
        print('CALL: h.check_in(["Alice", "Wonder"], True)\n',
              h.check_in(["Alice", "Wonder"], True), test_sep, sep="")
        print('CALL: h.check_in(["Alex"], True)\n', h.check_in(["Alex"], True), test_sep,
              sep="")
        print('PRINT h:\n', h, test_sep, sep="")
        print('CALL: h.check_in(["Oded", "Shani"], False)\n',
              h.check_in(["Oded", "Shani"], False), test_sep, sep="")
        print('CALL: h.check_in(["Oded", "Shani"], False)',
              h.check_in(["Oded", "Shani"], False), test_sep, sep="")
        print('CALL: h.check_out("Liat")\n', h.check_out("Liat"), test_sep, sep="")
        print('CALL: h.check_out("Liat")\n', h.check_out("Liat"), test_sep, sep="")
        print('PRINT h:\n', h, test_sep, sep="")


    
    test_hotel()
