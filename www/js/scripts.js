document.getElementById('msg-form').addEventListener('submit', async function (event) {
    event.preventDefault();
    const formData = new FormData(this);
    const responseContainer = document.getElementById('response');
    try{
        const response = await fetch('/', {
            method: 'POST',
            body: formData,
        });

        if(response.ok) {
            const result = await response.json();
            responseContainer.textContent = 'Success: ' + JSON.stringify(result);
        } else {
            const errorText = await response.text();
            responseContainer.textContext = 'Error: ' + errorText;
        }
    } catch(error) {
        responseContainer.textContent = 'Error: ' + error.messsage; 
    }
});
