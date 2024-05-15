from selenium import webdriver
from selenium.webdriver import ActionChains
from selenium.webdriver.common.by import By
import requests
import csv
import time

driver = webdriver.Chrome('./chromedriver.exe')
driver.implicitly_wait(5)
driver.get('https://wordforest.cn/book?id=5')

with open("zhuanba.csv","w",encoding='utf-8') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['word_id','english','chinese','US_phon','UK_phon','audio_name'])
    idx = 0
    for i in range(407):
        tr_list = driver.find_elements(By.CSS_SELECTOR,'#app > section:nth-child(3) > '
                                                       'div.container.is-max-desktop.is-flex.is-list > div.is-body.is-shadow '
                                                       '> table > tbody > tr')
        for tr in tr_list:
            eng = tr.find_element(By.CSS_SELECTOR, 'td:nth-child(2) > strong > a').text
            chn = ''
            ps = tr.find_elements(By.CSS_SELECTOR,'td.explain > p')
            for p in ps:
                chn += p.find_element(By.CSS_SELECTOR, 'i').text + ' '
                spans = p.find_elements(By.CSS_SELECTOR,'span')
                for span in spans:
                    chn += span.text + ' '
            # 假设您的定位元素的代码类似如下所示
            phonetic_elements = tr.find_elements(By.CSS_SELECTOR, '.phonetic')
            US_phon = ''
            UK_phon = ''
            for cnt, ph in enumerate(phonetic_elements):
                if cnt == 0:
                    US_phon = phonetic_elements[0].text
                if cnt == 1:
                    UK_phon = phonetic_elements[1].text


            audio_src = tr.find_element(By.CSS_SELECTOR,'audio').get_attribute('src')
            audio_name = eng + ".mp3"

            writer.writerow([idx,eng,chn,US_phon,UK_phon,audio_name])
            idx += 1
            print(idx)
            response = requests.get(audio_src)
            with open('./audios/' + audio_name, "wb") as file:
                file.write(response.content)
            button = driver.find_element(By.CSS_SELECTOR,'#app > section:nth-child(3) > '
                                                         'div.container.is-max-desktop.is-flex.is-list > '
                                                         'div.is-body.is-shadow > table > tfoot > tr > th > div > div '
                                                         '> button:nth-child(3)')
        button.click()
        time.sleep(5)

