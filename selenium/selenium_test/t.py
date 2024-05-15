from selenium import webdriver
from selenium.webdriver import ActionChains
from selenium.webdriver.common.by import By
import requests
import csv
import time
import random

driver = webdriver.Chrome('./chromedriver.exe')
driver.implicitly_wait(30)
driver.get('https://wordforest.cn/book?id=2')

button = driver.find_element(By.CSS_SELECTOR,'#app > section:nth-child(3) > '
                                                         'div.container.is-max-desktop.is-flex.is-list > '
                                                         'div.is-body.is-shadow > table > tfoot > tr > th > div > div '
                                                         '> button:nth-child(3)')
time.sleep(1)
if button:
    button.click()
print(input())