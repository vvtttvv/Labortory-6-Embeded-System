# Lab 6.2.1 ON-OFF cu histereza - Analiza + Diagrame

## 1) Verificare conformitate pe cerinte suplimentare

Stare actuala proiect (conform codului):

1. Modularizare (control, achizitie, actionare, IO): DA
2. Diagrame de control si flux de date: LIPSEAU in proiect, adaugate mai jos
3. Debouncing software pentru intrari digitale: PARTIAL
4. FreeRTOS taskuri separate pentru achizitie/control/actionare/afisare: PARTIAL
5. Sincronizare taskuri (mutex/semafor/coada): DA (mutex)
6. Afisare SetPoint/Value/Output/Enable pe STDIO: DA (Serial)
7. Reutilizare module, fara duplicari: DA
8. Testare scenarii/tranzitii: PARTIAL (nu era documentata)
9. Documentare arhitectura, interfete HW/SW, diagrame: PARTIAL (completata aici)
10. Monitorizare in timp real cu Serial Plotter: DA

Observatii cheie:

- Taskurile sunt 2 (control + report), nu 4 separate (acquire/control/actuate/display).
- Intrarea digitala exista in schema (switch), dar in varianta curenta este nefolosita in runtime.
- Pentru conformitate maxima la punctul FreeRTOS, se poate separa task_control in: task_acquire + task_control + task_actuate.

---

## 2) Diagrama structurala (A.1.4)

```mermaid
flowchart LR
    DHT[DHT11 Sensor] --> DHTDRV[dht_driver]
    POT[Potentiometer A0] --> SPDRV[setpoint_input]
    CTRL[hysteresis_control]
    ACTDRV[actuator_driver] --> LED[LED/Releu]

    DHTDRV --> APP[app_runtime tasks]
    SPDRV --> APP
    APP --> CTRL
    CTRL --> APP
    APP --> ACTDRV

    APP <--> STATE[app_state + mutex]
    APP --> IODRV[io_driver / STDIO]
    IODRV --> UART[Serial Plotter / Monitor]

    MAIN[src/main.cpp] --> APP
```

---

## 3) Arhitectura pe nivele HW-SW (A.1.4)

```mermaid
flowchart TB
    subgraph APP_L[Application Layer]
        APP[app_runtime]
        LOGIC[hysteresis_control]
        STATE[app_state]
    end

    subgraph SRV_L[Service Layer]
        DHTS[dht_driver]
        SPS[setpoint_input]
        ACTS[actuator_driver]
    end

    subgraph ECAL_L[ECAL / HAL]
        IO[io_driver]
    end

    subgraph HW_L[Hardware]
        MCU[Arduino UNO]
        DHT[DHT11]
        POT[POT A0]
        ACT[LED/Releu]
        UART[USB Serial]
    end

    APP_L --> SRV_L
    APP_L --> ECAL_L
    SRV_L --> ECAL_L
    ECAL_L --> HW_L
```

---

## 4) Diagrama de flux algoritm ON-OFF cu histereza (A.1.5)

```mermaid
flowchart TD
    S([Start periodic control]) --> R1[Read DHT: temp, humidity]
    R1 --> P1{Reading plausible?}
    P1 -- No --> F1[Mark fail count]
    P1 -- Yes --> U1[Update measured values]

    F1 --> R2[Read SetPoint from potentiometer]
    U1 --> R2

    R2 --> C1[Compute thresholds:
    ON = SP - H
    OFF = SP + H]

    C1 --> D1{Measured < ON ?}
    D1 -- Yes --> O1[Output = ON]
    D1 -- No --> D2{Measured > OFF ?}
    D2 -- Yes --> O2[Output = OFF]
    D2 -- No --> O3[Keep previous output]

    O1 --> A1[Write actuator]
    O2 --> A1
    O3 --> A1

    A1 --> T1[Update shared state + sample id]
    T1 --> E([Wait until next period])
```

---

## 5) Diagrama de stari pentru actuator (A.1.5)

```mermaid
stateDiagram-v2
    [*] --> OFF

    OFF --> ON: measured < setpoint - H
    ON --> OFF: measured > setpoint + H

    OFF --> OFF: setpoint-H <= measured <= setpoint+H
    ON --> ON: setpoint-H <= measured <= setpoint+H
```

---

## 6) Diagrama de secventa FreeRTOS (A.1.5)

```mermaid
sequenceDiagram
    participant Tctrl as task_control
    participant DHT as dht_driver
    participant SP as setpoint_input
    participant HYS as hysteresis_control
    participant ACT as actuator_driver
    participant ST as app_state+mutex
    participant Trpt as task_report
    participant IO as io_driver/UART

    loop PERIOD_CONTROL_MS
        Tctrl->>DHT: read(temp, hum)
        Tctrl->>SP: read setpoint
        Tctrl->>ST: lock mutex
        Tctrl->>HYS: step(measured, setpoint, H, prev)
        HYS-->>Tctrl: output
        Tctrl->>ACT: set(output)
        Tctrl->>ST: update shared state
        Tctrl->>ST: unlock mutex
    end

    loop PERIOD_REPORT_MS
        Trpt->>ST: lock mutex
        Trpt->>ST: copy state
        Trpt->>ST: unlock mutex
        Trpt->>IO: print SetPoint/Value/Humidity/Output/Enable
    end
```

---

## 7) Interfete HW/SW (pentru documentare)

- DHT11
  - HW: pin digital definit in config
  - SW: dht_driver_init, dht_driver_read

- Potentiometru
  - HW: A0
  - SW: setpoint_input_init, setpoint_input_read_c

- Actuator (LED/releu)
  - HW: pin digital output
  - SW: actuator_driver_init, actuator_driver_set

- STDIO serial
  - HW: UART USB
  - SW: io_uart_begin, io_printf, io_uart_print_report

---

## 8) Scenarii minime de test si criterii de validare (A.1.3/A.1.8)

1. Test histereza ON
- Conditie: Value < SetPoint - H
- Asteptat: Output=1, actuator ON

2. Test histereza OFF
- Conditie: Value > SetPoint + H
- Asteptat: Output=0, actuator OFF

3. Test banda moarta
- Conditie: SetPoint-H <= Value <= SetPoint+H
- Asteptat: Output pastreaza starea anterioara

4. Test variatie setpoint din potentiometru
- Conditie: rotire POT
- Asteptat: SetPoint in serial se modifica continuu in intervalul configurat

5. Test robustete DHT (fail)
- Conditie: intrerupere simulata citire
- Asteptat: warning periodic, sistemul continua executia taskurilor

6. Test concurenta taskuri
- Conditie: rulare 2+ minute
- Asteptat: fara blocari mutex, fara reset neasteptat

7. Test serial plotter
- Conditie: monitorizare in timp real
- Asteptat: apar seriile SetPoint, Value, Output, Enable

---

## 9) Recomandari pentru punctaj maxim

1. Separati explicit taskurile in 4: acquire, control, actuate, report.
2. Reintroduceti intrare digitala reala (switch) cu debouncing software activ.
3. Adaugati tabel de trasabilitate: cerinta -> test -> rezultat.
4. Adaugati capturi Plotter pentru minim 3 scenarii:
   - incalzire ON,
   - oprire OFF,
   - banda de histereza.
