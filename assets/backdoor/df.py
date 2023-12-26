from cryptography.fernet import Fernet
from PIL import Image
import io
def decrypt_image(encrypted_image, key):
    
    with open(encrypted_image, 'rb') as file:
        encrypted_data = file.read()

    
    f = Fernet(key)
    decrypted_data = f.decrypt(encrypted_data)

  
    decrypted_image = Image.open(io.BytesIO(decrypted_data))
    
   
    decrypted_image.save('decrypted_image.png')
    

if __name__ == "__main__":
    
    encrypted_image = "encoded.bin" 
    encryption_key ='zlMg5K3TobbFh_8l7doDT_408rH7Md_W3Oc1yKX1FrA='

    decrypt_image(encrypted_image, encryption_key)

