@ECHO OFF
:: This batch file is used to create the HSDATALOG Motor Control NanoEdgeAI data set
ECHO HSDATALOG Motor Control NanoEdgeAI data set creation
ECHO.

:: Check if acquisition_folder is provided
if "%~1"=="" (
    echo ERROR: Acquisition folder required. Launch this script by passing the acquisation folder.
    echo Example: MC_AI_dataset_creation ^<your acquisation folder^>
    PAUSE
    exit /b 1
)

set acquisation_folder=%1
set "exported_folder=%acquisation_folder%_Exported"

:: Section 1: Run hsdatalog_data_export_by_tags.py
ECHO ===========================================
ECHO Run hsdatalog_data_export_by_tags ...
ECHO ===========================================
python -m hsdatalog_data_export_by_tags %acquisation_folder% -s fast_mc_telemetries -r -f CSV
python -m hsdatalog_data_export_by_tags %acquisation_folder% -s iis3dwb_acc -r -f CSV
:: Section 2: Data Export complete
ECHO ===========================================
ECHO Data Export complete
ECHO ===========================================
:: Section 3: Run MC_AI_dataset_creation.py
ECHO ===========================================
ECHO Run MC_AI_dataset_creation ...
ECHO ===========================================
python -m MC_AI_dataset_creation %exported_folder%
ECHO ===========================================
ECHO MC_AI_dataset_creation complete
ECHO ===========================================

:: Open the exported folder
start "" "%exported_folder%"

PAUSE