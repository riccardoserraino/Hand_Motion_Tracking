# Muscle Synergy Analysis of Hand Motion Using sEMG and Optical Tracking
## Overview
This project presents a dual-modality framework for analyzing human hand motion using:

- Surface Electromyography (sEMG): to capture muscular activity.

- Optical Motion Tracking (Vicon): to capture kinematic data.

By combining these modalities, the system extracts muscle synergies through Sparse Non-Negative Matrix Factorization (sNMF) and compares them with 3D kinematic closure estimates.

Applications include rehabilitation, prosthetics, human–robot interaction, and gesture recognition.





## Project Structure
### code
The section containing all the scripts used for the analysis.
- **analyze** : folder containing scripts to visualize the main mathematical concepts used.
- **dataset** : folder containing the data aquired from the Vicon environment and from the BioGAP armband.
- **ikpy** : folder containing the ikpy library, directly downloaded into the project folder to manually make slight changes.
- **results** : folder containing result for all the different analysis carried out.
- **tests** : folder containing draft tests when we started the analysis of the dataset.
- **utils** : folder containing all the functions personally created for making the main scripts easier to understand.
- **config.py** : file that imports all the libraries used in the project.
- **motion_tracking.py** : file containing the final 3D model of the hand pipeline.
- **sigma_flexion.py** : file containing the final comparison of the closure level of the hand (more repetitions included).
- **single_sigma_flexion.py** : file containing the final comparison of the closure level of the hand.
- **single_synergy_extraction.py** : file containing the synergy extraction pipeline.
- **synergy_extraction.py** : file containing the synergy extraction pipeline (more repetitions included).


  
### nodes
The folder containing ROS nodes for acquiring data for the Vicon environment.
For the BioGAP armband refer to the github link contained in the 'info.txt' file of this folder.

### setup_pics
The folder coontaining photos of the environment developed and material used.





## Author 
Riccardo Serraino

Contributor: Giuliano Livi

Bachelor’s Thesis in Automation Engineering
University of Bologna, July 2025

Supervisors: Prof. Roberto Meattini, Alessandra Bernardini, Mattia Orlandi, Prof. Simone Benatti
