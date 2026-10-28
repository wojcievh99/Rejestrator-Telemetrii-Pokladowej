# RejestratorTelemetriiPokladowej

Zadanie rekruacyjne do Simle Simba Awionika Software\
(nie będę streszczał wymagań, gdyż są one w zadaniu)

## Instrukcja uruchomienia projektu

Po kompilacji wyświetli się program testowy na konsoli, dzięki któremu można przetestować program.
Domyślnie napisałem 5 testów:\
	1. Pobiera ostatnią ramkę w momencie gdy nie ma żadnego zapisu\
    2. Odczytuje wysokość (i inne dane) po wystąpieniu awarii barometru\
    3. Dodaje nowy odczyt barometru i odczytuje wysokość po zmianie statusu na LANDED\
    4. Odczytuje wysokość po wystąpieniu kolejno: awarii barometru i wylądowaniu\
    5. Zapisuje 17 ramek i odczytuje cały bufor kołowy\

## Struktura Projektu

Klasy i pliki:\
    1. TelemetryBlackBox.h (.cpp)\
    2. Buffer.h (.cpp)\
    3. tests.h (.cpp)\

## Specyfikacja plików

### TelemetryBlackBox

Domyślnie dodana klasa rejestratora telemetrii. Z jej poziomu obsługuje się cały program.

### Buffer

Moduł, w którym zaimplementowane są wszystkie operacje na bitach (pakowanie, odpakowywanie ramki).
Jest zaimplementowany zgodnie ze wzorcem singleton, aby mieć pewność, że istnieje tylko jeden bufor kołowy.
Korzysta z niej TelemetryBlackBox, aby pakować ramki do bufora, rozpakowywać ramki i logować je na konsoli.

### Tests

Implementacja 5 domyślnych testów.