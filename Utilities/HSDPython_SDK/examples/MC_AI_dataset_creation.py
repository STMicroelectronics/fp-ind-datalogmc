#!/usr/bin/env python
# coding: utf-8 
# *****************************************************************************
#  * @file    MC_AI_dataset_creation.py
#  * @author  SRA
# ******************************************************************************
# * @attention
# *
# * Copyright (c) 2022 STMicroelectronics.
# * All rights reserved.
# *
# * This software is licensed under terms that can be found in the LICENSE file
# * in the root directory of this software component.
# * If no LICENSE file comes with this software, it is provided AS-IS.
# *
# *
# ******************************************************************************
#

import os
import csv
import time

import numpy as np
import pandas as pd
import click

DECIMATION = 4

@click.command()
@click.argument('main_folder', type=click.Path(exists=True))

def MC_AI_dataset_creation(main_folder):
    if not os.path.isdir(main_folder):
        print("The provided path is not a directory.")
        return

    class_file_paths = []

    # Walk through the directory
    for dirpath, dirnames, filenames in os.walk(main_folder):
        # Initialize the variables to None for each subdirectory
        fast_telemetries_data_path = None
        iis3dwb_data_path = None
        class_file_path = None

        # Get the last part of the dirpath which is the folder name
        folder_name = os.path.basename(dirpath)
        # Construct the class file path
        class_file_path_candidate = os.path.join(dirpath, folder_name + ".csv")

        # Iterate through each file in the directory
        for filename in filenames:
            if "fast_mc" in filename:
                fast_telemetries_data_path = os.path.join(dirpath, filename)
            elif "iis3dwb" in filename:
                iis3dwb_data_path = os.path.join(dirpath, filename)

        # If both file paths are found and the class file path candidate exists, call the dataset creation function
        if fast_telemetries_data_path and iis3dwb_data_path:
            class_file_path = class_file_path_candidate
            dataset_creation(fast_telemetries_data_path, iis3dwb_data_path, class_file_path)
            class_file_paths.append(class_file_path)

    # After generating all files, correct the length of all class_file_path CSVs
    if class_file_paths:
        truncate_csv_files_to_min_length(class_file_paths)

def extract_n_elements_before_indices(values, indices, n):
    # initialize an empty list to store the results
    result = []

    # loop over each index in the array
    for i in indices:
        # extract n elements before the index
        start_index = i - n
        end_index = i
        if start_index < 0:
            start_index = 0
        extracted_values = values[start_index:end_index]
        # add the extracted values to the result list
        result.append(extracted_values)

    return result

def find_closest_minor_indices(values, candidates):
    # sort candidate array in ascending order
    candidates_sorted = np.sort(candidates)

    # initialize an array to store the indices of the closest minor values
    indices = np.zeros_like(values, dtype=int)

    # loop over each element in values
    for i, v in enumerate(values):
        # find the index of the closest minor value in candidates
        index = np.searchsorted(candidates_sorted, v, side='left') - 1
        # handle edge cases where v is smaller than the smallest value in candidates
        if index < 0:
            index = 0
        # store the index in the result array
        indices[i] = index

    return indices

def csv_to_array(file_path):
    # load the CSV file into a NumPy array, skipping the first row
    array = np.loadtxt(file_path, delimiter=',', skiprows=1, dtype=np.float32)

    return array

def array_decimation(array, decimation, axis):
    # reshape the input array into a 3D array
    array_3d = np.reshape(array, (axis, -1, decimation))

    # compute the average of every decimation samples along the second dimension
    decimated_array = np.mean(array_3d, axis=1)

    # flatten the decimated array and return it
    return decimated_array.flatten()

def decimate_1d_array_by_average(arr, decimation):
    """
    Decimates a 1D NumPy array by computing the average of every `decimation` elements.
    
    Args:
    arr (numpy.ndarray): The input array to be decimated.
    decimation (int): The factor by which to decimate the array.
    
    Returns:
    numpy.ndarray: The decimated array.
    """
    return np.mean(arr.reshape(-1, decimation), axis=1)

def decimate_2d_array_by_average(arr, decimation):
    """
    Decimates a 2D NumPy array along the first axis by computing the average of every `decimation` rows.
    
    Args:
    arr (numpy.ndarray): The input array to be decimated.
    decimation (int): The factor by which to decimate the array.
    
    Returns:
    numpy.ndarray: The decimated array.
    """
    return np.mean(arr.reshape(-1, decimation, arr.shape[1]), axis=1)

def decimate_array_by_average(arr, decimation):
    """
    Decimates a 1D or 2D NumPy array along the first axis by computing the average of every `decimation` rows or elements.
    
    Args:
    arr (numpy.ndarray): The input array to be decimated.
    decimation (int): The factor by which to decimate the array.
    
    Returns:
    numpy.ndarray: The decimated array.
    """
    if arr.ndim == 1:
        return decimate_1d_array_by_average(arr, decimation)
    elif arr.ndim == 2:
        return decimate_2d_array_by_average(arr, decimation)
    else:
        raise ValueError("Input array must be 1D or 2D")

def decimate_array_list_by_average(arr_list, decimation):
    """
    Decimates a list of 2D NumPy arrays along the first axis by computing the average of every `decimation` rows.
    
    Args:
    arr_list (list): The list of input arrays to be decimated.
    decimation (int): The factor by which to decimate the arrays.
    
    Returns:
    list: The list of decimated arrays.
    """
    return [decimate_array_by_average(arr, decimation) for arr in arr_list]

def concatenate_lists(arr_list1, arr_list2):
    # Check that the two lists have the same length
    if len(arr_list1) != len(arr_list2):
        raise ValueError("The two lists must have the same length.")
    
    # Concatenate each array in the two lists along the second axis
    concatenated_list = [np.concatenate((arr1, arr2), axis=1) 
                         for arr1, arr2 in zip(arr_list1, arr_list2)]
    return concatenated_list

def flatten_arrays(arr_list):
    # Reshape each array in the list to a 1D array
    flattened_list = [arr.reshape(-1) for arr in arr_list]
    
    return flattened_list

            
def dataset_creation(fast_telemetry_file_path, iis3dwb_acc_file_path, output_data_set_file_path):

    fast_telemetry = csv_to_array(fast_telemetry_file_path)
    iis3dwb_acc = csv_to_array(iis3dwb_acc_file_path)
    
    #extract data
    fast_telemetry_data = fast_telemetry[:, 1:]
    iis3dwb_acc_data = iis3dwb_acc[:, 1:]

    #extract timestamp
    fast_telemetry_ts = fast_telemetry[:,0]
    iis3dwb_acc_ts = iis3dwb_acc[:,0]

    #extract timestamp each fast_telemetry packet dimension x decimation
    fast_telemetry_decimated_ts = fast_telemetry_ts[1024*DECIMATION::1024*DECIMATION]
    iis3dwb_acc_decimated_idx = find_closest_minor_indices(fast_telemetry_decimated_ts, iis3dwb_acc_ts)
    fast_telemetry_decimated_idx = np.arange(1024*DECIMATION, len(fast_telemetry_data), 1024*DECIMATION)

    fast_telemetry_data_decimated_in =  extract_n_elements_before_indices(fast_telemetry_data, fast_telemetry_decimated_idx, 1024*DECIMATION)
    iis3dwb_acc_decimated_in = extract_n_elements_before_indices(iis3dwb_acc_data, iis3dwb_acc_decimated_idx, 256*DECIMATION*DECIMATION)

    fast_telemetry_data_decimated_out = decimate_array_list_by_average(fast_telemetry_data_decimated_in,DECIMATION)
    iis3dwb_acc_decimated_out = decimate_array_list_by_average(iis3dwb_acc_decimated_in,DECIMATION)

    aggregated_data = concatenate_lists(iis3dwb_acc_decimated_out, fast_telemetry_data_decimated_out)

    aggregated_data_flatten = flatten_arrays(aggregated_data)
    
    # Open the output file in append mode
    with open(output_data_set_file_path, 'w',  newline='') as output_file:

        # Create a CSV writer for the output file
        writer = csv.writer(output_file)

        # Write the aggregated data to the output file
        for row in aggregated_data_flatten:
            writer.writerow(row)

    print(output_data_set_file_path, "CREATED")


def truncate_csv_files_to_min_length(csv_file_paths):
    # Determine the minimum length among the CSV files
    min_length = float('inf')
    for file_path in csv_file_paths:
        data = np.genfromtxt(file_path, delimiter=',', skip_header=1)  # Assuming there's a header
        min_length = min(min_length, data.shape[0])

    # Truncate each CSV file to the minimum length
    for file_path in csv_file_paths:
        # Load the data
        with open(file_path, 'r') as f:
            header = f.readline()  # Read the header line
            data = np.genfromtxt(f, delimiter=',', skip_header=0)  # Already skipped the header

        # Truncate the data
        truncated_data = data[:min_length]

        # Write the truncated data back to the CSV file
        with open(file_path, 'w') as f:
            f.write(header)  # Write the header line
            np.savetxt(f, truncated_data, delimiter=',', fmt='%g')  # Use '%g' for floating-point format

if __name__ == '__main__':
    MC_AI_dataset_creation()
    