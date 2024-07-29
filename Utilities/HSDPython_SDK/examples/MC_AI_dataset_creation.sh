#!/bin/bash
# This bash script is used to create the HSDATALOG Motor Control NanoEdgeAI data set

echo "HSDATALOG Motor Control NanoEdgeAI data set creation"
echo

# Check if acquisition_folder is provided
if [ -z "$1" ]; then
    echo "ERROR: Acquisition folder required. Launch this script by passing the acquisition folder."
    echo "Example: ./MC_AI_dataset_creation.sh <your acquisition folder>"
    read -p "Press enter to continue"
    exit 1
fi

acquisition_folder=$1
exported_folder="${acquisition_folder}_Exported"

# Section 1: Run hsdatalog_data_export_by_tags.py
echo "==========================================="
echo "Run hsdatalog_data_export_by_tags ..."
echo "==========================================="
python3 -m hsdatalog_data_export_by_tags "$acquisition_folder" -s fast_mc_telemetries -r -f CSV
python3 -m hsdatalog_data_export_by_tags "$acquisition_folder" -s iis3dwb_acc -r -f CSV
# Section 2: Data Export complete
echo "==========================================="
echo "Data Export complete"
echo "==========================================="
# Section 3: Run MC_AI_dataset_creation.py
echo "==========================================="
echo "Run MC_AI_dataset_creation ..."
echo "==========================================="
python3 -m MC_AI_dataset_creation "$exported_folder"
echo "==========================================="
echo "MC_AI_dataset_creation complete"
echo "==========================================="

# Open the exported folder
xdg-open "$exported_folder"

read -p "Press enter to continue"