# Lab 6.2.1 - ON/OFF cu histereza (Varianta A)

Implementare modulara pentru Arduino Uno/Mega, cu FreeRTOS, pentru control
de temperatura pe baza DHT11 si actuator tip LED (in loc de releu).

## De ce LED ca actuator

- Nu ai releu disponibil.
- LED-ul este un actuator valid pentru demonstratie ON/OFF.
- Este usor de observat in timp real in Wokwi si pe placa reala.
- Codul pentru releu se poate substitui direct in `actuator_driver`.

## Arhitectura modulara

- `io_driver.*` - acces low-level pentru GPIO/ADC (pinMode, digitalWrite, analogRead).
- `dht_driver.*` - achizitie temperatura DHT11.
- `setpoint_input.*` - setpoint din potentiometru.
- `debounce_input.*` - debouncing software pentru intrare digitala (tilt switch).
- `hysteresis_control.*` - legea ON/OFF cu histereza.
- `actuator_driver.*` - actionare actuator (LED).
- `app_state.*` - stare partajata si mutex FreeRTOS.
- `main.cpp` - taskuri FreeRTOS: acquire/control/actuate/report.

## Legea de control

Pentru control tip incalzire:

- ON daca `Value < SetPoint - H`
- OFF daca `Value > SetPoint + H`
- altfel mentine starea anterioara

Unde `H = 0.8 C` (jumatate din banda de histereza).

## Taskuri FreeRTOS si recurenta

- `task_acquire` la 1000 ms: citeste DHT11, setpoint potentiometru, tilt debounced.
- `task_control` la 200 ms: aplica histereza pe valorile curente.
- `task_actuate` la 100 ms: actualizeaza actuatorul (LED).
- `task_report` la 1000 ms: trimite date pentru Serial Plotter.

Comunicarea intre taskuri se face prin variabila partajata `g_app_state`, protejata
de mutex (`g_state_mutex`).

## Mappare pini

- DHT11 DATA -> D2
- Potentiometru -> A0
- Tilt switch -> D3 (cu `INPUT_PULLUP`)
- Actuator LED -> D13

## Output pentru Arduino Serial Plotter

Linie seriala emisa:

`SetPoint:<float> Value:<float> Output:<0|1> Enable:<0|1>`

Acest format este compatibil cu Plotter.

## Cum testezi in Wokwi

1. Ruleaza proiectul pe env `uno`.
2. Modifica temperatura senzorului DHT11 din simulator.
3. Misca potentiometrul pentru schimbarea setpoint-ului.
4. Foloseste switch-ul (tilt simulare):
	- pozitia activa -> control enabled
	- pozitia inactiva -> actuator fortat OFF
5. Deschide Serial Plotter si verifica curbele pentru SetPoint/Value/Output.

