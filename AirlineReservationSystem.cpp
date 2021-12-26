#include "AirlineReservationSystem.h"

void AirlineReservationSystem::addPassenger(const std::string &firstname, const std::string &lastname)
{
    Passenger yolcu(firstname, lastname);
    passengers.insert(yolcu);
}

Passenger *AirlineReservationSystem::searchPassenger(const std::string &firstname, const std::string &lastname)
{
    return HELPERsearchPassenger(firstname, lastname, passengers.getRoot());
}

Passenger *AirlineReservationSystem::HELPERsearchPassenger(const std::string &firstname, const std::string &lastname, BSTNode<Passenger> *root)
{
    if (root == NULL)
        return NULL;
    if (root->data.getFirstname() == firstname && root->data.getLastname() == lastname)
        return &(root->data);
    if (root->left && HELPERsearchPassenger(firstname, lastname, root->left))
        return HELPERsearchPassenger(firstname, lastname, root->left);
    if (root->right && HELPERsearchPassenger(firstname, lastname, root->right))
        return HELPERsearchPassenger(firstname, lastname, root->right);
    return NULL;
}

void AirlineReservationSystem::addFlight(const std::string &flightCode, const std::string &departureTime, const std::string &arrivalTime, const std::string &departureCity, const std::string &arrivalCity, int economyCapacity, int businessCapacity)
{
    Flight flight_object(flightCode, departureTime, arrivalTime, departureCity, arrivalCity, economyCapacity, businessCapacity);
    flights.insert(flight_object);
}

std::vector<Flight *> AirlineReservationSystem::searchFlight(const std::string &departureCity, const std::string &arrivalCity)
{
    std::vector<Flight *> result;
    HELPERsearchFlight(departureCity, arrivalCity, result, flights.getRoot());
    return result;
}

void AirlineReservationSystem::HELPERsearchFlight(const std::string &departureCity, const std::string &arrivalCity, std::vector<Flight *> &result, BSTNode<Flight> *root)
{
    if (root == NULL)
        return;
    if (root->data.getDepartureCity() == departureCity && root->data.getArrivalCity() == arrivalCity)
        result.push_back(&(root->data));
    if (root->left)
        HELPERsearchFlight(departureCity, arrivalCity, result, root->left);
    if (root->right)
        HELPERsearchFlight(departureCity, arrivalCity, result, root->right);
}

void AirlineReservationSystem::issueTicket(const std::string &firstname, const std::string &lastname, const std::string &flightCode, TicketType ticketType)
{
    Passenger *yolcu = searchPassenger(firstname, lastname);
    if (yolcu == NULL)
        return;

    BSTNode<Flight> *ucus = FindFlightWithCode(flightCode, flights.getRoot());
    if (ucus == NULL)
        return;

    if (ticketType == economy && (ucus->data).getEconomyCapacity() > 0)
    {
        Ticket bilet(yolcu, &(ucus->data), ticketType);
        (ucus->data).addTicket(bilet);
    }
    if (ticketType == business && (ucus->data).getBusinessCapacity() > 0)
    {
        Ticket bilet(yolcu, &(ucus->data), ticketType);
        (ucus->data).addTicket(bilet);
    }
}

BSTNode<Flight> *AirlineReservationSystem::FindFlightWithCode(const std::string &flightCode, BSTNode<Flight> *root) const
{
    if (root == NULL)
        return NULL;
    if ((root->data).getFlightCode() == flightCode)
        return root;
    if (root->left && FindFlightWithCode(flightCode, root->left))
        return FindFlightWithCode(flightCode, root->left);
    if (root->right && FindFlightWithCode(flightCode, root->right))
        return FindFlightWithCode(flightCode, root->right);
    return NULL;
}

void AirlineReservationSystem::saveFreeTicketRequest(const std::string &firstname, const std::string &lastname, const std::string &flightCode, TicketType ticketType)
{
    Passenger *yolcu = searchPassenger(firstname, lastname);
    if (yolcu == NULL)
        return;

    BSTNode<Flight> *ucus = FindFlightWithCode(flightCode, flights.getRoot());
    if (ucus == NULL)
        return;

    Ticket bilet(yolcu, &(ucus->data), ticketType);
    freeTicketRequests.enqueue(bilet);
}

void AirlineReservationSystem::executeTheFlight(const std::string &flightCode)
{
    BSTNode<Flight> *ucus = FindFlightWithCode(flightCode, flights.getRoot());
    if (ucus == NULL)
        return;

    Ticket temp_ticket;

    int eco = (ucus->data).getEconomyCapacity();
    int bus = (ucus->data).getBusinessCapacity();
    int queue_size = freeTicketRequests.size();

    while (queue_size > 0)
    {
        temp_ticket = freeTicketRequests.dequeue();

        if (temp_ticket.getFlight() == &(ucus->data) && temp_ticket.getTicketType() == economy)
        {
            if (eco > 0)
            {
                (ucus->data).addTicket(temp_ticket);
                eco--;
            }
            else
                freeTicketRequests.enqueue(temp_ticket);
        }
        else if (temp_ticket.getFlight() == &(ucus->data) && temp_ticket.getTicketType() == business)
        {
            if (bus > 0)
            {
                (ucus->data).addTicket(temp_ticket);
                bus--;
            }
            else
                freeTicketRequests.enqueue(temp_ticket);
        }
        else
            freeTicketRequests.enqueue(temp_ticket);
        queue_size--;
    }

    (ucus->data).setCompleted(1);
}

void AirlineReservationSystem::print() const
{
    std::cout << "# Printing the airline reservation system ..." << std::endl;

    std::cout << "# Passengers:" << std::endl;
    passengers.print(inorder);

    std::cout << "# Flights:" << std::endl;
    flights.print(inorder);

    std::cout << "# Free ticket requests:" << std::endl;
    freeTicketRequests.print();

    std::cout << "# Printing is done." << std::endl;
}
