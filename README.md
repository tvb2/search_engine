# LeoT is a final project for a C++ developer beginner course.

[Objective](#objective)

[General overview](#general-overview)

[Features](#features)

[Installation](#installation)

## Objective
<p>Create a CLI search server with following capabilities</p>
<ul>
  <li>search performed over text documents in the same location as executable file</li>
  <li>  Using JSON interface (<a href="https://github.com/nlohmann/json">nlohmann_json library</a>) for:</li>
  <ul>
    <li>  <b>program configuration</b> is stored in a local config.json formatted cofig file</li>
    <li>  <b> search requests</b> taken from another requests.json file </li>
    <li>  <b>search results</b> are placed answers.json file</li>
  </ul>
  <li>Separate class for <b>periodic indexing</b> of a database is performed based on configuration settings</li>
  <li><b>Unit testing</b> performed using Google tests module</li>
</ul>

## General overview
<p>Upon startup InvertedIndex class instance starts looking for a 'database' directory and searches all .txt files in it. Creates a list of files, reads them in and performs indexing.</p>
<p>After that JSON module initializes and attempts to open up config file and reads configuration settings as well as updates list of files for the database. Search requests are read from a requests.json file </p>
<p>Then Search Server is started and performes search over the index, handing over results to JSON module and recorded to answers.json file. Additionally results are displayed in the terminal</p>
<p>
  After this initial search start background check of file database to be indexed in three detached threads
  <ul>
    <li><b>monitor</b> thread of a JSON module that checks if time since last indexting doesn't exceed a setting in the config file. Sets a bool flag when this time exceeds the setting and signals other modules that re-indexing is required</li>
    <li><b>updateIndex</b> thread of InvertedIndex class receives the bool flag and performs reindexing when the flag is set. It performs indexing and sets another bool flag to signal JSON module that indexing was complete</li>
    <li><b>updateJSON</b> thread of a JSON module receives the flag from InvertedIndex class and updates file list in the config.json file. It also updates timestamp of when indexing was performed</li>
  </ul> 
  </p>
  <p>A command line interface also runs in the background ready for user commands. User has a choice of commands:
      <ul>
          <li>index. manually command the program to re-index database</li>
          <li>search. manually command the program to perform search. At this command the program will read requests from requests.json file and display results in the teminal only</li>
          <li>exit. stop executing the program</li>
      </ul> 
  </p>

## Features
<p>Nlohmann library used to conveniently handle JSON format.</p>
<p>File list is stored in a map for quick access by path. This however leads to doc_id not corresponding directly to file names. It is easy to find which file corresponds to doc_id (and it is implemented when displaying search results in the terminal. </p>
<p>Indexing of database is performed in multiple threads. Number of threads is determined dynamically during runtime by hardware concurrency.</p>
<p>Monitoring and automatic database indexing is performed in the background using threads (detached). Time between indexing is defined in config.json in minutes. Initially, all re-indexing is performed over all files. However, in future can implement re-indexing of only new/modified files which should considerably reduce time for indexing. Initially, a database of about 1100 files of less than 1000 words each was used and indexing in maximum 4 threads takes about 95 seconds.</p>
<p>Number of search results to be written to answers.json is defined by config.json setting.</p>

## Installation
<p>Compile the program in Debug or Release modes. CMake is configured to copy files: config.json, requests.json, libstdc++-6.dll and 'database' folder to the build directory to allow program operation 'out of the box'. 'database' folder contains several text files for testing purposes.</p>
