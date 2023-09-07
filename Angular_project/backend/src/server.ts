import express, {  Router } from 'express';
import cors from 'cors'
import bodyParser from 'body-parser'
import mongoose from 'mongoose';
import userRouter from './routes/user.routes';
import examinationRouter from './routes/examination.routes';

// npm install bootstrap ( frontend )
// npm install express multer ( backend ) 


const app = express();
const multer = require('multer');
const path = require('path');

app.use(cors());
app.use(bodyParser.json());

mongoose.connect("mongodb://127.0.0.1:27017/projekat")
mongoose.connection.once('open', () => {
    console.log("db connection ok")
})
const router = Router()

router.use('/user', userRouter)
router.use('/examinations', examinationRouter)
app.use('/', router)

app.get('/', (req, res) => res.send('Hello World!'));
app.listen(4000, () => console.log(`Express server running on port 4000`));


