import streamlit as st
import pandas as pd

st.set_page_config(
    page_title="Travel Package Manager",
    page_icon="🌍",
    layout="wide"
)

st.title("🌍 Travel Package Manager")
st.write("Find travel destinations, weather conditions, required clothing, distance, and estimated cost.")

packages = pd.DataFrame([
    {
        "Destination": "Goa",
        "State/Country": "India",
        "Weather": "Hot and Humid",
        "Temperature (°C)": 31.0,
        "Required Clothing": "Cotton clothes, sunglasses, hat and slippers",
        "Distance (km)": 650.0,
        "Travel Cost per Person": 2500.0,
        "Hotel Cost per Day": 1800.0,
        "Food Cost per Day": 800.0,
        "Available Days": 10
    },
    {
        "Destination": "Manali",
        "State/Country": "Himachal Pradesh",
        "Weather": "Cold and Snowy",
        "Temperature (°C)": 5.0,
        "Required Clothing": "Jacket, sweater, gloves, woollen cap and boots",
        "Distance (km)": 2200.0,
        "Travel Cost per Person": 7000.0,
        "Hotel Cost per Day": 2500.0,
        "Food Cost per Day": 1000.0,
        "Available Days": 12
    },
    {
        "Destination": "Mysuru",
        "State/Country": "Karnataka",
        "Weather": "Pleasant",
        "Temperature (°C)": 24.0,
        "Required Clothing": "Comfortable cotton clothes and walking shoes",
        "Distance (km)": 180.0,
        "Travel Cost per Person": 1200.0,
        "Hotel Cost per Day": 1200.0,
        "Food Cost per Day": 600.0,
        "Available Days": 7
    },
    {
        "Destination": "Ooty",
        "State/Country": "Tamil Nadu",
        "Weather": "Cool and Cloudy",
        "Temperature (°C)": 15.0,
        "Required Clothing": "Light jacket, jeans and comfortable shoes",
        "Distance (km)": 270.0,
        "Travel Cost per Person": 1800.0,
        "Hotel Cost per Day": 1500.0,
        "Food Cost per Day": 700.0,
        "Available Days": 8
    },
    {
        "Destination": "Rajasthan",
        "State/Country": "India",
        "Weather": "Hot and Dry",
        "Temperature (°C)": 35.0,
        "Required Clothing": "Loose cotton clothes, scarf, sunglasses and sunscreen",
        "Distance (km)": 1800.0,
        "Travel Cost per Person": 6000.0,
        "Hotel Cost per Day": 2000.0,
        "Food Cost per Day": 900.0,
        "Available Days": 10
    }
])

menu = st.sidebar.radio(
    "Menu",
    [
        "View Travel Packages",
        "Weather and Clothing",
        "Cost Calculator",
        "Add New Package"
    ]
)

if menu == "View Travel Packages":
    st.subheader("Available Travel Packages")

    search = st.text_input("Search destination")

    if search:
        filtered = packages[
            packages["Destination"].str.lower().str.contains(search.lower())
        ]
    else:
        filtered = packages

    st.dataframe(filtered, use_container_width=True)

elif menu == "Weather and Clothing":
    st.subheader("Weather and Required Clothing")

    destination = st.selectbox(
        "Select destination",
        packages["Destination"].unique()
    )

    selected = packages[packages["Destination"] == destination].iloc[0]

    st.info(
        f"**Destination:** {selected['Destination']}\n\n"
        f"**Weather:** {selected['Weather']}\n\n"
        f"**Temperature:** {selected['Temperature (°C)']} °C\n\n"
        f"**Required Clothing:** {selected['Required Clothing']}"
    )

elif menu == "Cost Calculator":
    st.subheader("Travel Cost Calculator")

    destination = st.selectbox(
        "Select destination",
        packages["Destination"].unique()
    )

    selected = packages[packages["Destination"] == destination].iloc[0]

    days = st.number_input(
        "Number of travel days",
        min_value=1,
        max_value=int(selected["Available Days"]),
        value=3
    )

    persons = st.number_input(
        "Number of persons",
        min_value=1,
        max_value=50,
        value=2
    )

    transport_cost = selected["Travel Cost per Person"] * persons
    hotel_cost = selected["Hotel Cost per Day"] * days * persons
    food_cost = selected["Food Cost per Day"] * days * persons
    total_cost = transport_cost + hotel_cost + food_cost

    st.success(
        f"**Destination:** {selected['Destination']}\n\n"
        f"**Transport Cost:** Rs. {transport_cost:,.2f}\n\n"
        f"**Hotel Cost:** Rs. {hotel_cost:,.2f}\n\n"
        f"**Food Cost:** Rs. {food_cost:,.2f}\n\n"
        f"### Total Cost: Rs. {total_cost:,.2f}"
    )

elif menu == "Add New Package":
    st.subheader("Add New Travel Package")

    destination = st.text_input("Destination")
    state = st.text_input("State/Country")
    weather = st.selectbox(
        "Weather condition",
        [
            "Hot and Humid",
            "Hot and Dry",
            "Cold and Snowy",
            "Cool and Cloudy",
            "Rainy",
            "Pleasant"
        ]
    )
    temperature = st.number_input("Temperature (°C)", value=25.0)
    clothing = st.text_input("Required clothing")
    distance = st.number_input("Distance (km)", value=100.0)
    travel_cost = st.number_input("Travel cost per person", value=1000.0)
    hotel_cost = st.number_input("Hotel cost per day", value=1000.0)
    food_cost = st.number_input("Food cost per day", value=500.0)
    available_days = st.number_input("Available days", min_value=1, value=5)

    if st.button("Add Package"):
        new_package = pd.DataFrame([{
            "Destination": destination,
            "State/Country": state,
            "Weather": weather,
            "Temperature (°C)": temperature,
            "Required Clothing": clothing,
            "Distance (km)": distance,
            "Travel Cost per Person": travel_cost,
            "Hotel Cost per Day": hotel_cost,
            "Food Cost per Day": food_cost,
            "Available Days": available_days
        }])

        packages = pd.concat([packages, new_package], ignore_index=True)

        st.success(f"Package for {destination} added successfully.")
        st.dataframe(packages, use_container_width=True)
